
#include <stdio.h>
#include "structDefinition.h"
#include "heapify.h"


void swap2(struct batch **arr, int x, int y) ;
//void heapify(struct batch **arr, int n, int i, int order);


void swap2(struct batch **arr, int x, int y) {
struct batch * temp = arr[x];
arr[x] = arr[y];
arr[y] = temp;
}


// building heap
void heapify(struct batch **arr, int n, int i, int order)
{
    int largest = i;  // Initialize largest as root
    int l = 2*i + 1;  // left = 2*i + 1
    int r = 2*i + 2;  // right = 2*i + 2

    // If left child is larger than root
    if(order == 0) {
    if (l < n && arr[l]->buf[arr[l]->index] < arr[largest]->buf[arr[largest]->index])
        largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r]->buf[arr[r]->index] < arr[largest]->buf[arr[largest]->index])
        largest = r;
    }
     else {
    if (l < n && arr[l]->buf[arr[l]->index] > arr[largest]->buf[arr[largest]->index])
        largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r]->buf[arr[r]->index] > arr[largest]->buf[arr[largest]->index])
        largest = r;
}

    // If largest is not root
    if (largest != i)
    {
        swap2(arr, i, largest);

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest, order);
    }
}