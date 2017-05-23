#include "list_mtm1.h"
#include "node.h"
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>

//List def
struct list_t {
    Node list_head;
    CopyListElement copy_function;
    FreeListElement free_function;
    int size;
    Node list_iterator;
};

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
    new_list -> iterator = NULL;
    new_list -> list_head = NodeCreate(NULL, copyElement, freeElement);
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
    Node copy_iterator = list -> list_head, dest_iterator = 
                                                GetLast(new_list -> list_head);
    while(copy_iterator != NULL) {
        NodeResult add_node = NodeInsert(dest_iterator,
                                                      CopyNode(copy_iterator));
        if(add_node != NODE_SUCCESS) {
            free(new_list);
            return NULL;
        }
        copy_iterator = GetNext(copy_iterator);
        dest_iterator = GetLast(new_list -> list_head);
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
    list -> list_iterator = list -> list_head;
    return(GetData(list -> list_iterator));
} 

ListElement listGetNext(List list) {
    if(list == NULL || list -> size == 0 || list -> iterator == NULL || 
                                        GetNext(list -> iterator) == NULL) {
        return NULL;
    }
    list -> iterator = GetNext(list -> iterator);
    return GetData(list -> iterator);
}

ListElement listGetCurrent(List list) {
    if(list == NULL || list -> size == 0 || list -> iterator == NULL) {
        return NULL;
    }
    return GetData(list -> iterator);
}


ListResult listInsertFirst(List list, ListElement element) {
    if(list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    Node to_insert = NodeCreate(element, list -> copy_function,
                                                        list -> free_function);
    if(to_insert == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    assert(list -> list_head != NULL);
    NodeResult insert_result = NodeInsert(list -> list_head, to_insert); 
    return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element) {
    if(list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    Node to_insert = NodeCreate(element, list -> copy_function,
                                                        list -> free_function);
    if(to_insert == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    Node iterator = GetLast(list -> list_head);
    assert(list -> list_head != NULL);
    NodeResult insert_result = NodeInsert(iterator, to_insert);
    return LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list, ListElement element) {
     if(list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    Node to_insert = NodeCreate(element, list -> copy_function,
                                                        list -> free_function);
    if(to_insert == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
}