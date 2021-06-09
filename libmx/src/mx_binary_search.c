#include "libmx.h"

int mx_binary_search(char **arr, int size, const char *s, int *count) {
    int left = 0;
    int right = size - 1;
    int search = -1;
    while(left <= right) {
        int mid = (left + right) / 2;
        (*count)++;
        if(mx_strcmp(arr[mid], s) == 0) {
            search = mid;
            break;
        }
        if(mx_strcmp(arr[mid], s) > 0 ) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }
    if(search == -1) {
        (*count) = 0;
    }
    return search;
}

