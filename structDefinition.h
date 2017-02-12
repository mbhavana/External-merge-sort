#ifndef __STRUCT_H__
#define __STRUCT_H__

struct Info {
// Total number of integers to be sorted.
int total;
// Elements in one batch while sorting.
int element_per_batch;
// Since total may not be fully divisible by elements per batch.
int element_last_batch;
// Number of batches in which file is divided.
int num_batches;
// Pass number.
//int pass_num;
// output file.
char *output;
// 0 - ascending 1- descending
int order;
// Process memory limit in  MBs
int limit;
// will be used in external merge sort in case of more than one passes.
int offset;
};

// Batch structure for external merge sort.
struct batch {
// Elements left in that buffer.
int elements_left;
// Seek value where next element will be present in  file.
int64_t seek_value;
// BUffer array to store array of integers while doing merge sort.
int64_t *buf;
// Number of elements in buffer.
int64_t buf_size;
// Current index of buffer to be read.
int64_t index;
// Not required just for debugging. Can be removed.
int num;
};

#endif