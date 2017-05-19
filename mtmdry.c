#include <stdio.h>
#include <assert.h>

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


int main() {
    
}
