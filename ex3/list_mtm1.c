#include "list_1mtm.h"
#include "node.h"
#include <stdbool.h>

struct list_t {
    Node list_head;
    CopyListElement copy_function;
    FreeListElement free_function;
};

List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
    if(copy_element == NULL || free_element == NULL) {
        return NULL;
    }
    List new_list = malloc(sizeof(*new_list));
    if(new_list == NULL) {
        return NULL;
    }
    new_list -> copy_function = copyElement;
    new_list -> free_function = freeElement;
}