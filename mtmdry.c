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

//Node struct.
typedef struct node_t* Node;
struct node_t {
    int n;
    Node next;
};

//This function creats and returns a new node with value of x.
Node createNode (int x) {
    Node ptr = malloc(sizeof(*ptr));
    if(!ptr) {
        return NULL;
    }
    ptr -> n = x;
    ptr -> next = NULL;
    return ptr;
}

//This function destroys a given node and frees it.
Node destroyNode(Node head, Node to_delete) {
    Node temp = head;
    while(temp -> next != to_delete) {
        temp = temp -> next;
    }
    temp -> next = to_delete -> next;
    to_delete -> next = NULL;
    free(to_delete);
    return temp -> next;
}

//This function gets a list and creates and returns a new one with same values.
Node copyList(Node list) {
    Node list_it = list, new_list, new_list_it;
    new_list = createNode(list -> n);
    list_it = list_it -> next;
    new_list_it = new_list;
    while(list_it != NULL) {
        new_list_it -> next = createNode(list_it -> n);
        new_list_it = new_list_it -> next;
        list_it = list_it -> next;
    }
    new_list_it -> next = NULL;
    return new_list;
}

//This function concats two lists, and returns the new concated list.
Node simpleConcatLists(Node head1, Node head2) {
    if(head1 == NULL) {
        return copyList(head2);
    }
    if(head2 == NULL) {
        return copyList(head1);
    }
    Node new_list1 = copyList(head1);
    Node new_list2 = copyList(head2);
    Node temp = new_list1;
    while(temp -> next != NULL) {
        temp = temp -> next;
    }
    temp -> next = new_list2;
    return new_list1;
}

/* This function gets a list and a condition function and returns a new list
    with values that satify the condition. */
Node filterList(Node head ,bool (*conditionFunc)(int, void*), void* param) {
    Node temp = head;
    while(temp != NULL) {
        if(conditionFunc(temp -> n, param)) {
            temp = temp -> next;
            continue;
        } else {
            Node to_delete = temp;
            if(to_delete == head) {
                head = head -> next;
                temp = head;
                to_delete -> next = NULL;
                free(to_delete);
            } else {
                temp = destroyNode(head, to_delete);
            }
        }
    }
    return head;
}

/* This function gets 2 lists and a condition function and returns a new list
    which is the concation of the 2 filtered list by the condition. */
Node concatLists(Node head1, Node head2, bool (*conditionFunc)(int, void*), 
                                                                void* param) {
    if(conditionFunc == NULL) {
        return NULL;
    }
    Node concatedFilteredList = simpleConcatLists(head1, head2);
    concatedFilteredList=filterList(concatedFilteredList, conditionFunc, param);
    return concatedFilteredList;
}

/* Function that checks and returns whether a number is odd or even according to
    the given param (1 for even 0 for odd). */
bool checkParity(int x, void* param) {
    int conv_param = *(int*)param;
    if(conv_param) {
        return (x%2 == 0);
    }
    else {
        return (x%2 ==1);
    }
}

/* Function that checks if a number is prime. Note: no param needed, just for
    the compability with other functions. */
bool checkPrime(int x, void* param) {
    if(x < 2) {
        return false;
    }
    for(int i = 2; i < x; i++) {
        if(x % i == 0) {
            return false;
        }
    }
    return true;
}

//Main according to what we were asked...
int main() {
    Node head1 = createNode(1);
    head1 -> next = createNode(2);
    head1 -> next -> next = createNode(3);
    Node head2 = createNode(4);
    head2 -> next = createNode(5);
    head2 -> next -> next = createNode(6);
    head2 -> next -> next -> next = createNode(7);
    int p1 = 0;
    Node odd_node = concatLists(head1, head2, checkParity, (void *)&p1);
    Node prime_node = concatLists(head1, head2, checkPrime, NULL);
    destroyList(head1);
    destroyList(head2);
    destroyList(odd_node);
    destroyList(prime_node);
    return 0;
}
