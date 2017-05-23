#include <stdbool.h>
#ifndef _NODE_H
#define _NODE_H

typedef struct list_node* Node;
typedef void* Element;
typedef void* ListFilterKey;
typedef Element (*CopyFunction)(Element);
typedef void (*FreeFunction)(Element);
typedef int(*CompareFunction)(Element, Element);
typedef bool(*FilterFunction)(Element, ListFilterKey);


typedef enum {
    NODE_SUCCESS, NODE_NULL_ARGUMENT, NODE_OUT_OF_MEMORY
} NodeResult;

Node NodeCreate (Element element, CopyFunction copy_function,
                                                FreeFunction free_function);
NodeResult NodeDestroy (Node target, Node head);
Node GetNext (Node node);
NodeResult NodeInsert (Node node, Node target);
Node CopyNode (Node node);
void* GetData (Node node);
int CompareNode(Node first, Node second, CompareFunction compare_element);
bool FilterNode(Node node, ListFilterKey key, FilterFunction filter_element);
Node GetLast (Node node);

#endif //NODE_H_





