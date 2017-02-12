# External-merge-sort

main.c source code has all other required source code files included.
randomizedQuickSort.c is the base line program which reads all the contents in the file without memory limit. 
 quicksort.c is naïve sorting program which is used in sorting phase for external merge sort. It creates a temporary file which is given as input to merge phase.
heapify.c program builds by rearranging heap structure of all the records during merge phase.
heapsort.c performs heap sort at the beginning of External merge sort.
Ext_merge_sort.c reads blocks of data within memory limit and merges them. The output is written to output file specified by user.
2.	Makefile contains shell commands required for execution. This creates two executable files: program1 program2
Execution of program
At the prompt type:
$ make
2.	The execution of above command gives a message:
Complier named: program has been compiled
3.	Give the following command at the prompt:
./program1 extsort.input output_file1 order memory_limit
./program2 extsort.input output_file2 order memory_limit

4.	First argument is name of your input file.
5.	Second argument is name of your output file which contains external merge sort records.
6.	Third argument is order in which records should be sorted.
Give value 0 for ascending order.
Give value 1 for descending order.
           7.   Fourth argument is the memory limit. 

          

