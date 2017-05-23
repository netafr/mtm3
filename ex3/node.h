#ifndef _NODE_H
#define _NODE_H

typedef struct list_node* Node;
typedef void* Element;
typedef Element (*CopyFunction)(Element);
typedef void (*FreeFunction)(Element);
typedef int(*CompareFunction)(Element, Element);

typedef enum {
    NODE_SUCCESS, NODE_NULL_ARGUMENT, NODE_OUT_OF_MEMORY
} NodeResult;

Node NodeCreate (Element element, CopyFunction copy_function, 
                CompareFunction, compare_function ,FreeFunction free_function);
NodeResult NodeDestroy (Node target, Node head);
Node GetNext (Node node);
Node CopyNode (Node node);
int CompareNode (Node first, Node second);
#endif //NODE_H_





