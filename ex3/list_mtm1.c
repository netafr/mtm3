#include "list_mtm1.h"
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>

//Node def
typedef struct list_node {
    ListElement node_data;
    struct list_node* next;
} *Node;

//List def
struct list_t {
    Node list_head;
    CopyListElement copy_function;
    FreeListElement free_function;
    int size;
    Node list_iterator;
};

//Node errors
typedef enum {
    NODE_SUCCESS, NODE_NULL_ARGUMENT, NODE_OUT_OF_MEMORY
} NodeResult;

/* IMPORTANT. We implemented the list with the first element as "dummy", so it
    has no real meaning expect in the implemnation itself. */


/*  The function receives an element, a copy function and a free function,
    and allocates memory for the node. Initializes the node for the given */
static Node NodeCreate(ListElement element, CopyListElement copy_function) {
    Node node = calloc(1, sizeof(*node));
    if(node == NULL) {
        return NULL;
    }
    if(element == NULL) {
        node -> node_data = NULL;
        return node;
    }
    node -> node_data = copy_function(element);
    if(node -> node_data == NULL) {
        free(node);
        return NULL;
    }
    return node;
}

/* The function receives a node to destroy, and a node which is the "first", 
    to start from. It frees the node destroyed and the element the node contains 
    if the "first" node is NULL, we return a NULL argument error. */
static NodeResult NodeDestroy(Node target, Node head, 
                                                FreeListElement free_element) {
    if(head == NULL) {
        return NODE_NULL_ARGUMENT;
    }
    if(target == NULL) {
        return NODE_SUCCESS;
    }
    Node iterator = head;
    if(target == head) {
        head = head -> next;
        return NODE_SUCCESS;
    }
    while(iterator -> next != target) {
        iterator = iterator -> next;
    }
    iterator -> next = target -> next;
    target -> next = NULL;
    free_element((target -> node_data));
    free(target);
    return NODE_SUCCESS;
}

/* The function receives a node and a second one (target) to insert right after 
   the first one. Returns NULL argument error if the first node is null, and 
    success elsewise. We assume the target node is not NULL.  */
static NodeResult NodeInsertAfter(Node node, Node target) {
    if(node == NULL) {
        return NODE_NULL_ARGUMENT;
    }
    assert (target != NULL);
    target -> next = node -> next;
    node -> next = target;
    return NODE_SUCCESS;
}

/* The function receives a node and a second one (target) to insert right before 
   the first one. Returns NULL argument error if the first node is null, and 
    success elsewise. We assume the target node is not NULL.  */
static NodeResult NodeInsertBefore(Node current, Node target, Node head) {
    if(head == NULL) {
        return NODE_NULL_ARGUMENT;
    }
    Node iterator = head;
    while(iterator -> next != current) {
        iterator = iterator -> next;
    }
    iterator -> next = target;
    target -> next = current;
    return NODE_SUCCESS;
}

/* The function receives a node to copy, and returns a new copy of the node. */
static Node CopyNode(Node node, CopyListElement copy_element) {
    if(node == NULL) {
        return NULL;
    }
    return NodeCreate(node -> node_data, copy_element);
}

/* The function receives a node, and returns the last Node linked in the nodes'
chain of nodes. */
static Node GetLast (List list) {
    if (list == NULL) {
        return NULL;
    }
    Node temp = list -> list_head;
    while (temp -> next != NULL)  {
        temp = temp -> next;
    }
    return temp;
}


/* The function gets a pointer to copy function, pointer to free function 
    create and returns a new list with the initalized values and a empty node.
    The head of the list is demi-node. */
List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
    if(copyElement == NULL || freeElement == NULL) {
        return NULL;
    }
    List new_list = malloc(sizeof(*new_list));
    if(new_list == NULL) {
        return NULL;
    }
    new_list -> copy_function = copyElement;
    new_list -> free_function = freeElement;
    new_list -> size = 0;
    new_list -> list_iterator = NULL;
    new_list -> list_head = NodeCreate(NULL, copyElement);
    if(new_list -> list_head == NULL) {
        free(new_list);
        return NULL;
    }
    return new_list;
}


/* The function gets a list, create and returns a new list with the same 
    (copied) elements. If the given list is NULL we return NULL, also in any
    case of failed memory allocation. */
List listCopy(List list) {
    if(list == NULL) {
        return NULL;
    }
    List new_list = listCreate(list -> copy_function, list -> free_function);
    if(new_list == NULL) {
        return NULL;
    }
    Node copy_iterator = list -> list_head -> next, 
                                        dest_iterator = GetLast(new_list);
    while(copy_iterator != NULL) {
        //Insert new copied node.
        NodeResult add_node = NodeInsertAfter(dest_iterator,
                                CopyNode(copy_iterator, list -> copy_function));
        if(add_node != NODE_SUCCESS) {
            free(new_list);
            return NULL;
        }
        if(list -> list_iterator == copy_iterator) {
            new_list -> list_iterator = dest_iterator -> next;
        }
        copy_iterator = copy_iterator -> next;
        dest_iterator = dest_iterator -> next;
        (new_list -> size)++; //Increase new list size.
    }
    return new_list;
}

// The function returns the size of the given list. Returns -1 if list is NULL. 
int listGetSize(List list) {
    if(list == NULL) {
        return -1;
    }
    return list -> size;
}

/* The function sets the inner iterator to the first element of the given list.
    If list is NULL or empty we return NULL. */
ListElement listGetFirst(List list) {
    if(list == NULL || list -> size == 0) {
        return NULL;
    }
    list -> list_iterator = list -> list_head -> next;
    return(list -> list_iterator -> node_data);
} 

/* The function sets the iterator to the next element in the list and returns 
    the element. If list is NULL, empty or the iterator is illegal or the last
    element we return NULL. It gets a list. */
ListElement listGetNext(List list) {
    if(list == NULL || list -> size == 0 || list -> list_iterator == NULL) {
        return NULL;
    }
    list -> list_iterator = list -> list_iterator -> next;
    if(list -> list_iterator == NULL) {
        return NULL;
    }
    return(list -> list_iterator -> node_data);
}

/* The function returns the current element in the list that the iterator of the 
    given list is on. If list is NULL or empty or the iterator is 
    illegal we return NULL. */
ListElement listGetCurrent(List list) {
    if(list == NULL || list -> size == 0 || list -> list_iterator == NULL) {
        return NULL;
    }
    return(list -> list_iterator -> node_data);
}

/*The function gets a list and an element, and inserts the element as the first
    element of the list. If the list is NULL it returns LIST_NULL_ARGUMENT, 
    if there is memory problem returns LIST_OUT_OF_MEMORY, otherwise return
    LIST_SUCCESS. Increases the size of the list. */
ListResult listInsertFirst(List list, ListElement element) {
    if(list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    if(element == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    Node to_insert = NodeCreate(element, list -> copy_function);
    if(to_insert == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    assert(list -> list_head != NULL);
    NodeResult insert_result = NodeInsertAfter(list -> list_head, to_insert); 
    if(insert_result != NODE_SUCCESS) {
        return LIST_NULL_ARGUMENT;
    }
    (list -> size)++;
    return LIST_SUCCESS;
}

/*The function gets a list and an element, and inserts the element as the last
    element of the list. If the list is NULL it returns LIST_NULL_ARGUMENT, 
    if there is memory problem returns LIST_OUT_OF_MEMORY, otherwise return
    LIST_SUCCESS. Increases the size of the list. */
ListResult listInsertLast(List list, ListElement element) {
    if(list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    if(element == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    Node to_insert = NodeCreate(element, list -> copy_function);
    if(to_insert == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    Node iterator = GetLast(list);
    assert(list -> list_head != NULL);
    NodeResult insert_result = NodeInsertAfter(iterator, to_insert);
    if(insert_result != NODE_SUCCESS) {
        return LIST_NULL_ARGUMENT;
    }
    (list -> size)++;
    return LIST_SUCCESS;
}

/* The function gets a list and an element, and inserts the element before the
    current element pointed by the inner iterator.If the list is NULL it returns 
    LIST_NULL_ARGUMENT,if there is memory problem returns LIST_OUT_OF_MEMORY, 
    and if the iterator is illegal returns LIST_INVALID_CURRENT,
    otherwise return LIST_SUCCESS. Increases the size of the list */
ListResult listInsertBeforeCurrent(List list, ListElement element) {
    if(list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    if(element == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    if(list -> list_iterator == NULL) {
        return LIST_INVALID_CURRENT;
    }
    Node to_insert = NodeCreate(element, list -> copy_function);
    if(to_insert == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    NodeResult insert_result = NodeInsertBefore(list -> list_iterator,to_insert, 
                                                            list -> list_head);
    if(insert_result != NODE_SUCCESS) {
        return LIST_NULL_ARGUMENT;
    }
    (list -> size)++;
    return LIST_SUCCESS;
    
}

/* The function gets a list and an element, and inserts the element after the
    current element pointed by the inner iterator.If the list is NULL it returns 
    LIST_NULL_ARGUMENT,if there is memory problem returns LIST_OUT_OF_MEMORY, 
    and if the iterator is illegal returns LIST_INVALID_CURRENT,
    otherwise return LIST_SUCCESS. Increases the size of the list */
ListResult listInsertAfterCurrent(List list, ListElement element) {
    if(list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    if(element == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    if(list -> list_iterator == NULL) {
        return LIST_INVALID_CURRENT;
    }
    Node to_insert = NodeCreate(element, list -> copy_function);
    if(to_insert == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    NodeResult insert_result = NodeInsertAfter(list -> list_iterator, to_insert); 
    if(insert_result != NODE_SUCCESS) {
        return LIST_NULL_ARGUMENT;
    }
    (list -> size)++;
    return LIST_SUCCESS;
}

/* The function gets a list and removes from the list the current element 
    pointed by the inner iterator. If the list is NULL it returns 
    LIST_NULL_ARGUMENT and if the iterator is illegal returns 
    LIST_INVALID_CURRENT,  otherwise return LIST_SUCCESS.
    Decreases the size of the list */
ListResult listRemoveCurrent(List list) {
    if(list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    if(list -> list_iterator == NULL) {
        return LIST_INVALID_CURRENT;
    }
    NodeResult destroy_result = NodeDestroy(list -> list_iterator, 
                                    list -> list_head, list -> free_function);
    if(destroy_result != NODE_SUCCESS) {
        return LIST_NULL_ARGUMENT;
    }
    list -> list_iterator = NULL;
    (list -> size)--;
    return LIST_SUCCESS;
}

/* The function gets 2 nodes and swaps their data. */
static void NodeSwap(Node node1, Node node2) {
    ListElement temp = node1 -> node_data;
    node1 -> node_data = node2 -> node_data;
    node2 -> node_data = temp;
}

/* The function gets a list and a pointer to a compare function and bubble sorts 
    the list according to the compate function. */
static void BubbleSort(List list, CompareListElements compareElement) {
    assert(list != NULL && compareElement != NULL);
    int swapped;
    Node iterator;
    Node last_sorted = NULL;
    if(list -> list_head -> next == NULL) {
        return;
    }
    do  {
        swapped = 0;
        iterator = list -> list_head -> next;
        while(iterator -> next != last_sorted) {
            if(compareElement(iterator -> node_data, 
                                        iterator -> next -> node_data) > 0) {
                NodeSwap(iterator, iterator -> next);
                swapped = 1;
            }
            iterator = iterator -> next;
        }
        last_sorted = iterator;
    } while(swapped);
}

/* The function gets a list and a pointer to a compare function and sorts
    the given list. If the list or the funcion pointer is NULL we return 
    LIST_NULL_ARGUMENT, otherwise return LIST_SUCCESS. */
ListResult listSort(List list, CompareListElements compareElement) {
    if(list == NULL || compareElement == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    BubbleSort(list, compareElement);
    return LIST_SUCCESS;
}

/* The function gets a list, pointer to filter function and a filter key, and
    creates and returns a new list composed from the elements of the given list
    that fulfill the filter function condition with the given key. If either
    the list or the pointer to the function are NULL, or there is a memory 
    problem we return NULL, otherwise we return the new list. */
List listFilter(List list, FilterListElement filterElement, ListFilterKey key) {
    if(filterElement == NULL || list == NULL) {
        return NULL;
    }
    List filtered_list = listCreate(list -> copy_function, 
                                                    list -> free_function);
    if(filtered_list == NULL) {
        return NULL;
    }
    Node iterator = list -> list_head -> next, 
                                    dest = filtered_list -> list_head;
    while(iterator != NULL) {
        if(filterElement(iterator -> node_data, key)) {
            NodeResult add_node = NodeInsertAfter(dest, CopyNode(iterator, 
                                                        list -> copy_function));
            assert(add_node == NODE_SUCCESS);
            dest = dest -> next;
            (filtered_list -> size)++;
        }
        iterator = iterator -> next;
    }
    filtered_list -> list_iterator = filtered_list -> list_head -> next;
    return filtered_list;
}

/* The function gets a list and clears and frees the list's elements. If the 
    list is NULL we return LIST_NULL_ARGUMENT, otherwise LIST_SUCCESSS. */
ListResult listClear(List list) {
    if(list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    Node head = list -> list_head;
    while(head -> next != NULL) {
        NodeResult destroy_result = NodeDestroy(head -> next, head, 
                                                    list -> free_function);
        assert(destroy_result == NODE_SUCCESS);
    }
    list -> list_iterator = NULL;
    list -> size = 0;
    return LIST_SUCCESS;
}

/* The function gets a list and destroys it, meaning clearing all it's elements
    and freeing the list itself. If list is NULL we do nothing. */
void listDestroy(List list) {
    if(list == NULL) {
        return;
    }
    ListResult clear_result = listClear(list);
    assert(clear_result == LIST_SUCCESS);
    free(list -> list_head);
    free(list);
    return;
}
