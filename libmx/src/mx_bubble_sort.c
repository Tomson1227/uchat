#include "libmx.h"

int mx_bubble_sort(int *arr, int size)
{
    int tmp;
    int count = 0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                tmp = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = tmp;
                count++;
            }
        }
    }
    return count;
}
