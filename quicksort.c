#include <stdio.h>
#include <sys/time.h>
#include "quicksort.h"

void swap(int64_t *arr , int x, int y);

void swap(int64_t *arr , int x, int y) {
int64_t temp;
temp = arr[x];
arr[x] = arr[y];
arr[y] = temp;
}




void quicksort(int64_t *arr, int first, int last, int order) {
    int pivot,j,temp,i;
    struct timeval tv;
    gettimeofday(&tv, NULL);

     if(first<last){
     int rand_index = ((int)tv.tv_usec % (last-first+1)) + first;
     swap(arr, first, rand_index);
         pivot=first;
         i=first;
         j=last;

        while(i < j) {
         if(order == 0) {
             while(arr[i] <= arr[pivot] && i<last)
                 i++;

             while(arr[j] > arr[pivot])
                 j--;
         } else {
         while(arr[i] >= arr[pivot] && i<last)
                 i++;

            while(arr[j] < arr[pivot])
                j--;
         }

             if(i<j){
                 swap(arr, i, j);
             }
        }

        swap(arr, pivot, j);
        quicksort(arr,first,j-1, order);
        quicksort(arr,j+1,last, order);
    }
}
