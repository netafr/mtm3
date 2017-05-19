#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <malloc.h>
/*to add to git:
git add.
git commit -m "comment"
git push -u origin [branch]
*/

/*Function gets array, its size, a compare function. and the object to find
    the compare function returns positive number if the first value is larger
    than the second and vice versa. */
int binaryFind(void** arr, int arr_size, int(*compareFunc)(void *, void *), 
                                                            void* to_find) {
    int high = arr_size - 1;
    int low = 0;
    int mid = 0;
    while (low <= high) {
        mid = (high + low)/2;
        if(compareFunc(to_find, arr[mid]) == 0) {
            return mid;
        }
        else if(compareFunc(to_find, arr[mid]) > 0) {
            low = mid + 1;
        } else {
            assert(compareFunc(to_find, arr[mid]) < 0);
            high = mid - 1;
        }
    }
    return -1;
}


typedef struct node_t* Node;
struct node_t {
    int n;
    Node next;
};

Node simpleConcatLists(Node head1, Node head2) {
    if(head1 == NULL) {
        return head2;
    }
    if(head2 == NULL) {
        return head1;
    }
    Node new_list = head1;
    while(new_list -> next != NULL) {
        new_list = new_list -> next;
    }
    new_list -> next = head2;
    return head1;
}

void filterList(Node head ,bool (*conditionFunc)(int, void*), void* param) {
    Node temp = head;
    while(temp != NULL) {
        if(conditionFunc(temp -> n, param)) {
            temp = temp -> next;
            continue;
        } else {
            Node to_delete = temp;
            temp = to_delete -> next;
            to_delete -> next = NULL;
            //destroyList(to_delete);
        }
    }
}

Node concatLists(Node head1, Node head2, bool (*conditionFunc)(int, void*), 
                                                                void* param) {
    if(conditionFunc == NULL) {
        return NULL;
    }
    Node concatedFilteredList = simpleConcatLists(head1, head2);
    filterList(concatedFilteredList, conditionFunc, param);
    return concatedFilteredList;
}

Node createNode (int x) {
    Node ptr = malloc(sizeof(*ptr));
    if(!ptr) {
        return NULL;
    }
    ptr -> n = x;
    ptr -> next = NULL;
    return ptr;
}

bool checkEven(int x, void* param) {
    int conv_param = *(int*)param;
    if(conv_param) {
        return (x%2 == 0);
    }
    else {
        return (x%2 ==1);
    }
}

int main() {
    Node head1 = createNode(1);
    head1 -> next = createNode(2);
    head1 -> next -> next = createNode(3);
    Node head2 = createNode(4);
    head2 -> next = createNode(5);
    head2 -> next -> next = createNode(6);
    head2 -> next -> next -> next = createNode(7);
    int *p1 = malloc(sizeof(int)), *p2 = malloc(sizeof(int));
    *p1 = 1;
    *p2 = 2;
    Node even_node = concatLists(head1, head2, checkEven, (void *)p1);
    Node odd_node = concatLists(head1, head2, checkEven, (void *)p2);
}
