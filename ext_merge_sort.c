#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define TEMP_FILE "temp.txt"

// Page size = 4KB, Integer size = 16, Buffer size = 4096/16 = 512
#define BUF_SIZE 512
// Buffer size -- page size
#define BATCH_SIZE 4096
// size of integer
#define NUM_LEN 16
#include "structDefinition.h"
#include "heapsort.h"
#include "heapify.h"
#include "ext_merge_sort.h"



void external_merge_sort(struct Info * info);
int64_t get_next_seek_value(int64_t old, int skip);
int load_buf(struct batch *b, FILE *fin) ;

int64_t get_next_seek_value(int64_t old, int skip) {
return old + skip*(NUM_LEN + 1);
}



// Load integers in buffer during external merge sort.
int load_buf(struct batch *b, FILE *fin) {
int count = 0;
fseek(fin, b->seek_value, SEEK_SET);
while(b->elements_left > 0) {
fscanf(fin, "%lld", &b->buf[count++]);
--(b->elements_left);
if(count == b->buf_size) {
break;
}
}
b->index = 0;
b->buf_size = count;
b->seek_value = get_next_seek_value(b->seek_value, count);
return count != 0;
}
// External merge for a number of batches.
void external_merge_sort(struct Info * info) {
int num_batches = info->num_batches;
int loaded, i;

// Input file will be temporary file that contains sorted chunks.
FILE *fin = fopen(TEMP_FILE, "r");
FILE *fout;

// Depending upon if it is final pass or not.
// If it is final pass then write output into final output file provided as argument 2
// otherwise append output to temporary file. TEMP_FILE as defined above.
// Each pass will process chunks and create bigger chunks and append it to temp file.
// Since max size of temp file can be 2 times input file so max 2 passes are possible.
if(strcmp(info->output, TEMP_FILE) == 0) {
fout = fopen(info->output, "a");
} else {
fout = fopen(info->output, "w+");
}

// Creating array of buffers and an output buffer for external merge sort.
struct batch ** arr = (struct batch **) malloc(sizeof(struct batch *)* num_batches);
struct batch * outBatch = (struct batch *) malloc(sizeof(struct batch));

outBatch->buf_size = BUF_SIZE;
outBatch->buf = (int64_t *)malloc(sizeof(int64_t)*BUF_SIZE);
outBatch->index = 0;

// Initialising buffers with initial values
for(i=0; i<(num_batches-1); ++i) {
arr[i] = (struct batch *)malloc(sizeof(struct batch));
arr[i]->elements_left = info->element_per_batch;
arr[i]->buf_size = BUF_SIZE;
arr[i]->buf = (int64_t *)malloc(sizeof(int64_t)*BUF_SIZE);
arr[i]->num = i;
arr[i]->seek_value = info->offset + (i * info->element_per_batch * (NUM_LEN+1)) - 1;
if(arr[i]->seek_value < 0) arr[i]->seek_value = 0;

load_buf(arr[i], fin);
}

// Initialising buffer for last one. Since it may contain lesser number of integers.
arr[i] = (struct batch *)malloc(sizeof(struct batch));
arr[i]->elements_left = info->element_last_batch;
arr[i]->buf_size = BUF_SIZE;
arr[i]->buf = (int64_t *)malloc(sizeof(int64_t)*BUF_SIZE);
arr[i]->num = i;
arr[i]->seek_value = info->offset + (i * info->element_per_batch * (NUM_LEN+1)) - 1;
if(arr[i]->seek_value < 0) arr[i]->seek_value = 0;

load_buf(arr[i], fin);

heapSort(arr, num_batches, info->order);

// Merging all buffers.
while(num_batches > 0) {
// Adding smallest element in output buffer.
outBatch->buf[outBatch->index++] = arr[0]->buf[arr[0]->index];
// If output buffer is full. then print it into file.
if(outBatch->index == outBatch->buf_size) {
for(i=0; i<outBatch->index; ++i) {
fprintf(fout, "%016lld\n", outBatch->buf[i]);
}
outBatch->index = 0;
}

// smallest element is read from 0th buffer so incrementing index of buffer.
arr[0]->index++;

// If 0th buffer is empty then load it again.
if(arr[0]->index == arr[0]->buf_size) {
loaded = load_buf(arr[0], fin);
// If no element is loaded it means that buffer is empty now.
// remove that buffer from heap array.
if(loaded) {
heapify(arr, num_batches, 0, info->order);	
} else {
free(arr[0]->buf);
free(arr[0]);
arr[0] = arr[num_batches-1];
num_batches -= 1;
heapify(arr, num_batches, 0, info->order);
}
} else {
heapify(arr, num_batches, 0, info->order);
}
}

// When all input buffers are read. There can be elements in output buffer.
// writing them into file.
for(i=0; i<outBatch->index; ++i) {
fprintf(fout, "%016lld\n", outBatch->buf[i]);
}

free(outBatch->buf);
free(outBatch);

fclose(fin);
fclose(fout);
}


// Managing passes and then calling external merge sort.
int external_merge_sort_util(struct Info * info) {
int max = (1024*1024*info->limit)/BATCH_SIZE - 1; //B # of memory blocks
int j;

// If number of batches are more than maximum batches that can be sorted in one time
// using external merge sort in specified memory limit.
if(max < info->num_batches) {

int iter = info->num_batches/max;

if(info->num_batches%max) iter++;

struct Info * info2 = (struct Info *)malloc(sizeof(struct Info));
info2->total = info->total;
info2->element_per_batch = info->element_per_batch;
info2->element_last_batch = info->element_per_batch;
info2->num_batches = max;
info2->output = TEMP_FILE;
info2->order = info->order;
info2->limit = info->limit;
info2->offset = 0;

printf("num of batches: %d", info2->num_batches);

// Calling external merge sort taking max number of batches at a time out of number of batches.
for(int i=0; i<iter; ++i) {
if(i == iter-1) {
printf("%d last batch has: %d\n", info->element_per_batch, info->element_last_batch);
if(info->num_batches%max) {
info2->num_batches = info->num_batches%max;
printf("num of batches: %d", info2->num_batches);
}
info2->element_last_batch = info->element_last_batch;
}
external_merge_sort(info2);

info2->offset = get_next_seek_value(info2->offset, info2->element_per_batch * max);
}

// Temp file will contain 2 times input file. upper one will smaller batches and lower half with bigger batches.
// Now we have 3 bigger batches that will be sorted in one go.
// Only difference will be offset.
// Offset will be after total number of integers in input file.
info2->offset = get_next_seek_value(0, info2->total);
info2->element_per_batch = info->element_per_batch * max;
info2->element_last_batch = (info2->num_batches-1) * info->element_per_batch + info->element_last_batch;
info2->output = info->output;
info2->num_batches = iter;
external_merge_sort(info2);
return 2;
} else {
// If number of batches is lesser than max then only once external merge sort is required.
external_merge_sort(info);
return 1;
}
}