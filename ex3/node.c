#include "node.h"
#include <malloc.h>
#include <stdlib.h>

//Node def
struct list_node { 
    Element node_data;
    CopyFunction copy_element;
    FreeFunction free_element;
    Node next;
};

/*  The function receives an element, a copy function and a free function,
    and allocates memory for the node. Initializes the node for the given */
Node NodeCreate(Element element, CopyFunction copy_function,
                                                FreeFunction free_function) {
    Node node = malloc(sizeof(*node));
    if(node == NULL) {
        return NULL;
    }
    node -> copy_element = copy_function;
    node -> free_element = free_function;
    if(element == NULL) {
        node -> node_data = NULL;
        return node;
    }
    node -> node_data = malloc(sizeof(Element));
    if(node -> node_data) {
        free(node);
        return NULL;
    }
    node -> node_data = copy_function(element);
    return node;
}
/* The function receives a node to destroy, and a node which is the "first", 
    to start from. It frees the node destroyed and the element the node contains 
    if the "first" node is NULL, we return a NULL argument error. */
NodeResult NodeDestroy(Node target, Node head) {
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
    target -> free_element(target -> node_data);
    free(target);
    return NODE_SUCCESS;
}
/* The function receives a node, and returns the next one. */
Node GetNext (Node node) {
    if(node == NULL) {
        return NULL;
    }
    return node -> next;
}
/* The function receives a node to copy, and returns a new copy of the node.
   */
Node CopyNode(Node node) {
    if(node == NULL) {
        return NULL;
    }
    Node new_node = NodeCreate(node -> node_data, node -> copy_element, 
                                                    node -> free_element);
    return new_node;
}
/* The function uses the compare function to compare between
   the two Nodes received */
int CompareNode(Node first, Node second, CompareFunction compare_element) {
    return(compare_element(first -> node_data, second -> node_data));
}

/* The function uses the filter function to return whether the element stands
    in the filter standart. */
bool FilterNode(Node node, ListFilterKey key, FilterFunction filter_element) {
    return(filter_element(node -> node_data ,key));
}