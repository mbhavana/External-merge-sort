#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "quicksort.h"
#include "structDefinition.h"
#include "heapsort.h"
#include "heapify.h"
#include "ext_merge_sort.h"

#define TEMP_FILE "temp.txt"

// Page size = 4KB, Integer size = 16, Buffer size = 4096/16 = 512
#define BUF_SIZE 512
// Buffer size -- page size
#define BATCH_SIZE 4096
// size of integer
#define NUM_LEN 16

// Get integer from command line arguments.
int getInt(char *input) {
int n = 0, i =0;

while(input[i] != '\0') {
n = n*10 + input[i] - 48;
++i;
}

return n;
}



int main(int argc, char **argv) {
int64_t *nums = NULL;
FILE * fin;
FILE *ftemp;
struct Info *info = (struct Info *)malloc(sizeof(struct Info));

int cur, iter;
int i;
struct timeval tv;
int64_t quicksort_time = 0;
int64_t external_merge_sort_time = 0;
int64_t temp_time;
int max;

info->total = 0;
info->element_last_batch = 0;
info->num_batches = 0;

if(argc == 5) {
fin = fopen(argv[1], "r");
ftemp = fopen(TEMP_FILE, "w+");

info->limit = getInt(argv[4]);

if(argv[3][0] == '0') {
info->order = 0;
} else info->order = 1;

info->output = argv[2];

} else {
printf("Insufficient arguements.\n");
return 1;
}

if(!fin) {
printf("Input file you enetered doesnot exist. Please enter the correct name");
return 1;
}

// Calculating length of a batch for single quicksort so that process memory does not exceed limit.
info->element_per_batch = 1024 * 128 * info->limit ;//- 38400;
// Just for debugging, max batch size smaller so 2 passes thing can be verified.
//info->element_per_batch = 15000;

nums = (int64_t *) malloc(sizeof(int64_t)* info->element_per_batch);

// Getting time of day.
gettimeofday(&tv, NULL);
temp_time = tv.tv_sec*1000 + tv.tv_usec/1000;

// Reading batch from input file and then sorting it using quicksort and printing to temp file.
while(1) {
cur = 0;
if(feof(fin)) {
break;
}
while( cur < info->element_per_batch) {
nums[cur] = -1;
fscanf(fin, "%lld", &nums[cur]);
if(nums[cur] == -1) {
break;
}
++cur;
++info->total;
}

if(cur > 0 && cur != info->element_per_batch) {
info->element_last_batch = cur;
}
if(cur > 0)
++info->num_batches;
quicksort(nums, 0, cur-1, info->order);

for(iter = 0; iter < cur; ++iter) {
fprintf(ftemp, "%016lld\n", nums[iter]);
}
}

max = (1024*1024*info->limit)/ BATCH_SIZE - 1;

fclose(fin);
fclose(ftemp);
free(nums);

// If number of batches is more than square of max batches it means 3 passes are required.

if(info->num_batches > (max*max)) {
printf("File size is too big to be sorted using this.\n");
return 0;
}

printf("total values read: %d in %d batches\n", info->total, info->num_batches);

gettimeofday(&tv, NULL);
quicksort_time += tv.tv_sec*1000 + tv.tv_usec/1000 - temp_time;

if(info->element_last_batch == 0) info->element_last_batch = info->element_per_batch;

gettimeofday(&tv, NULL);
temp_time = tv.tv_sec*1000 + tv.tv_usec/1000;

printf("Passes required: %d\n", external_merge_sort_util(info));

gettimeofday(&tv, NULL);
external_merge_sort_time += tv.tv_sec*1000 + tv.tv_usec/1000 - temp_time;

printf("quicksort_time tooks: %lld secs\n", quicksort_time/1000);
printf("external merge sort tooks: %lld secs\n", external_merge_sort_time/1000);

return 0;
}
