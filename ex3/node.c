#include "node.h"
#include <malloc.h>
#include <stdlib.h>

//Node def
struct list_node { 
    Element node_data;
    CopyFunction copy_element;
    FreeFunction free_element;
    CompareFunction compare_element;
    Node next;
};

/*  The function gets an element, a copy function and a free function,
    and allocates memory for the node. Initializes the node for the given */
Node NodeCreate(Element element, CopyFunction copy_function, 
                CompareFunction compare_function, FreeFunction free_function) {
    Node node = malloc(sizeof(*node));
    if(node == NULL) {
        return NULL;
    }
    node -> copy_element = copy_function;
    node -> free_element = free_function;
    node -> compare_element = compare_function;
    node -> node_data = malloc(sizeof(Element));
    if(node -> node_data) {
        free(node);
        return NULL;
    }
    node -> node_data = copy_function(element);
    return node;
}
/* The function gets a node to destroy, and a node which is the "first", 
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
/* The function gets a node, and returns the next one. */
Node GetNext (Node node) {
    if(node == NULL) {
        return NULL
    }
    return node -> next;
}
/* The */
Node CopyNode(Node node) {
    Node new_node = malloc(sizeof(*new_node));
    if(new_node == NULL) {
        return NULL;
    }
    new_node -> copy_element = node -> copy_element;
    new_node -> free_element = node -> free_element;
    new_node -> compare_element = node -> compare_element;
    new_node -> node_data = node -> copy_element(node -> node_data);
    new_node -> next = NULL;
    return new_node;
}

int CompareNode(Node first, Node second) {
    return(first -> compare_element(first -> node_data, second -> node_data));
}