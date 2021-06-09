#include "libmx.h"

void swap(int* a, int* b) 
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
} 
  
int partition (int arr[], int low, int high, int *count) 
{ 
    int pivot = arr[high]; 
    int i = low;
    int j = high - 1;

    while(i != j) {
        while(arr[i] <= pivot && i != j)
            ++i;
        while(arr[j] > pivot && i != j)
            --j;

        if(i != j ) {
            swap(&arr[i], &arr[j]); 
            (*count)++;
        }
    }
    
    if(arr[i] > pivot) {
        swap(&arr[i], &arr[high]); 
        (*count)++;
        return i;
    }
    else
        return high;
} 

int mx_quicksort(int *arr, int left, int right)
{
    if (arr == NULL) {
        return -1;
    }

    int count = 0;

    if (left < right) 
    { 
        int pi = partition(arr, left, right, &count);
        
        count += mx_quicksort(arr, left, pi - 1); 
        count += mx_quicksort(arr, pi + 1, right); 
    } 
    return count;
}
