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

/*  The function receives an element, a copy function and a free function,
    and allocates memory for the node. Initializes the node for the given */
Node NodeCreate(ListElement element, CopyListElement copy_function) {
    Node node = malloc(sizeof(*node));
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
NodeResult NodeDestroy(Node target, Node head, FreeListElement free_element) {
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
NodeResult NodeInsertAfter(Node node, Node target) {
    if(node == NULL) {
        return NODE_NULL_ARGUMENT;
    }
    assert (target != NULL);
    target -> next = node -> next;
    node -> next = target;
    return NODE_SUCCESS;
}

NodeResult NodeInsertBefore(Node current, Node target, Node head) {
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
Node CopyNode(Node node, CopyListElement copy_element) {
    if(node == NULL) {
        return NULL;
    }
    return NodeCreate(node -> node_data, copy_element);
}

/* The function receives a node, and returns the last Node linked in the nodes'
chain of nodes. */
Node GetLast (List list) {
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


List listCopy(List list) {
    if(list == NULL) {
        return NULL;
    }
    List new_list = listCreate(list -> copy_function, list -> free_function);
    if(new_list == NULL) {
        return NULL;
    }
    Node copy_iterator = list -> list_head, dest_iterator = GetLast(new_list);
    while(copy_iterator != NULL) {
        NodeResult add_node = NodeInsertAfter(dest_iterator,
                                CopyNode(copy_iterator, list -> copy_function));
        if(add_node != NODE_SUCCESS) {
            free(new_list);
            return NULL;
        }
        copy_iterator = copy_iterator -> next;
        dest_iterator = dest_iterator -> next;
        (new_list -> size)++;
    }
    return new_list;
}

int listGetSize(List list) {
    if(list == NULL) {
        return -1;
    }
    return list -> size;
}

ListElement listGetFirst(List list) {
    if(list == NULL || list -> size == 0) {
        return NULL;
    }
    list -> list_iterator = list -> list_head -> next;
    return(list -> list_iterator -> node_data);
} 

ListElement listGetNext(List list) {
    if(list == NULL || list -> size == 0 || list -> list_iterator == NULL || 
                                        list -> list_iterator -> next == NULL) {
        return NULL;
    }
    list -> list_iterator = list -> list_iterator -> next;
    return(list -> list_iterator -> node_data);
}

ListElement listGetCurrent(List list) {
    if(list == NULL || list -> size == 0 || list -> list_iterator == NULL) {
        return NULL;
    }
    return(list -> list_iterator -> node_data);
}


ListResult listInsertFirst(List list, ListElement element) {
    if(list == NULL) {
        return LIST_NULL_ARGUMENT;
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

ListResult listInsertLast(List list, ListElement element) {
    if(list == NULL) {
        return LIST_NULL_ARGUMENT;
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

ListResult listInsertBeforeCurrent(List list, ListElement element) {
    if(list == NULL) {
        return LIST_NULL_ARGUMENT;
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

ListResult listInsertAfterCurrent(List list, ListElement element) {
    if(list == NULL) {
        return LIST_NULL_ARGUMENT;
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

void NodeSwap(Node node1, Node node2) {
    ListElement temp = node1 -> node_data;
    node1 -> node_data = node2 -> node_data;
    node2 -> node_data = temp;
}

void BubbleSort(List list, CompareListElements compareElement) {
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
                                        iterator -> next -> node_data) < 0) {
                NodeSwap(iterator, iterator -> next);
                swapped = 1;
            }
            iterator = iterator -> next;
        }
        last_sorted = iterator;
    } while(swapped);
}

ListResult listSort(List list, CompareListElements compareElement) {
    if(list == NULL || compareElement == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    BubbleSort(list, compareElement);
    return LIST_SUCCESS;
}

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
    list -> list_iterator = list -> list_head;
    list -> size = 0;
    return LIST_SUCCESS;
}

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
