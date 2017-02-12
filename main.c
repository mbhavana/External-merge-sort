/*
Final project: Implementation of External Merge sort
Name: Bhavana Maradani
student id: 01417051
email: bhavana_maradani@student.uml.edu

Source files: 

main.c - main program which performs external merge sort

quicksort.c - Sorts the input records.
heapsort.c - Sorting the heap structure
heapify.c - Biulds heap
ext_merge_sort.c - Merges the records in N/B groups


*/




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

#define TEMPORARY_OUTFILE "temporary_output.txt"
// Page size = 4KB, Record size = 16, Buffer size = 4096/16 = 512
#define records_per_block 512
// Buffer size -- page size
#define BLOCK_SIZE 4096
// size of integer
#define RECORD_LEN 16

// get the argument from command line and convert it to interger
int argTINT(char *input) {
int inp = 0, i =0;

while(input[i] != '\0') {
inp = inp*10 + input[i] - 48;
++i;
}

return inp;
}


/* ***********************MAIN PROGRAM******************************
Read the input file to perform External Merge sort.
Initially it performs Quick sort on the input file specifoed by user.
Divides the input file into N/B blocks each of size atmost B.
Sorts the data in chunks and outputs them to temporary file.

In merge phase reads B-1 blocks of data and merges all the groups.

*/
int main(int argc, char **argv) {

unsigned long *nums = NULL;

//Initializing input and temporary files
FILE * file_input;
FILE *temp_file;
struct sort_merge *sort_merge = (struct sort_merge *)malloc(sizeof(struct sort_merge));

int blocks; //initializing blocks variable for looking into blocks per group
int rec;
int i;
int max;


unsigned long duration2;



time_t quicksort_start_time, mergesort_start_time, quicksort_end_time, mergesort_end_time;



sort_merge->totalRecords = 0;
sort_merge->last_group_records = 0;
sort_merge->run_groups = 0;

if(argc == 5) {
file_input = fopen(argv[1], "r");
temp_file = fopen(TEMPORARY_OUTFILE, "w+");

sort_merge->memory_size = argTINT(argv[4]);

if(argv[3][0] == '0') {
sort_merge->sort_order = 0;
} else sort_merge->sort_order = 1;

sort_merge->result = argv[2];

} else {
printf("Please provide all the required arguments in the command line.\n");
return 1;
}


 if (file_input == NULL)
    {
        fprintf(stderr, "Could not open input file 'extsort.input' \n");
        
        exit(1);
    }
// number of records in buffer should not exceed memory limit
sort_merge->tuples_in_group = 1024 * 128 * sort_merge->memory_size ;


nums = (unsigned long *) malloc(sizeof(unsigned long)* sort_merge->tuples_in_group);


time(&quicksort_start_time);


/* 
Sort each N/B bloks in memory and write them to tempory output file
*/
printf("\nPerforming Quick sort on the input file....\n");
printf("________________________________________________\n");
while(1) {
		blocks = 0;
		if(feof(file_input)) {
		break;
		}
		while( blocks < sort_merge->tuples_in_group) {
			nums[blocks] = -1;
			fscanf(file_input, "%lld", &nums[blocks]);
			if(nums[blocks] == -1) {
			break;
		}
		++blocks;
		++sort_merge->totalRecords;
		}

		if(blocks > 0 && blocks != sort_merge->tuples_in_group) {
			sort_merge->last_group_records = blocks;
		}
		if(blocks > 0)
		++sort_merge->run_groups;
		quicksort(nums, 0, blocks-1, sort_merge->sort_order);

		for(rec = 0; rec < blocks; ++rec) {
			fprintf(temp_file, "%016lld\n", nums[rec]);
		}
		printf(".................................\n");
		printf("Records present per group : %d\n", rec);
		printf(".................................\n");
}

max = (1024*1024*sort_merge->memory_size)/ BLOCK_SIZE - 1;

//Closing all the opened files
fclose(file_input);
fclose(temp_file);
free(nums);



if(sort_merge->run_groups > (max*max)) {
printf("Input file size is too large to be handled..\n");
return 0;
}

//printf("Total number of records in the input file are: %d\n\n",sort_merge->totalRecords);
printf("Total number of groups for Quick sort: %d \n\n", sort_merge->run_groups);




time(&quicksort_end_time);

printf("\nMerging the blocks\n");
printf("___________________________\n");


if(sort_merge->last_group_records == 0) sort_merge->last_group_records = sort_merge->tuples_in_group;


time(&mergesort_start_time);



printf("Total number of Passes required to merge N/B groups : %d \n", external_sort(sort_merge));


time(&mergesort_end_time);


duration2 = difftime(quicksort_end_time, quicksort_start_time) + difftime(mergesort_end_time, mergesort_start_time);

printf("\nTotal execution time for External merge sort is : %lu secs\n", duration2);

return 0;
}
