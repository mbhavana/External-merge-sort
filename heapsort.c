#include <stdio.h>
#include "structDefinition.h"
#include "heapsort.h"
#include "heapify.h"



// to do heap sort for  array during start of external merge sort.
void heapSort(struct batch **arr, int n, int order)
{
    // Build heap (rearrange array)
    for (int i = n/2 - 1; i >= 0; i--)
        heapify(arr, n, i, order);
}