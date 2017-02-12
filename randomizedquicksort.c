/*

Final project: Implementation of External Merge sort
Name: Bhavana Maradani
student id: 01417051
email: bhavana_maradani@student.uml.edu


This program doesnot have memory limit
It reads the input data and assigns a random pivot 
Sorts the entire data according to pivot

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>



//void random_shuffle(unsigned long arr[], unsigned long numRecords);
void swap(unsigned long *a, unsigned long *b);
int partion(unsigned long arr[], int p, int r);
void quicksort(unsigned long arr[], int p, int q);


/*This function shuffles the elements in an array*
void random_shuffle(unsigned long arr[], unsigned long numRecords)
{
    srand(time(NULL));
    int i, j, temp;
    for (i = numRecords - 1; i > 0; i--)
    {
        j = rand()%(i + 1);
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}
*/
 
void swap(unsigned long *a, unsigned long *b)
{
    unsigned long temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

/*This function Partions the array*/
int partion(unsigned long arr[], int p, int r)
{
    int pivotIndex = p + rand()%(r - p + 1); //generates a random number as a pivot
    unsigned long pivot;
    int i = p - 1;
    int j;
    pivot = arr[pivotIndex];
    swap(&arr[pivotIndex], &arr[r]);
    for (j = p; j < r; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
 
    }
    swap(&arr[i+1], &arr[r]);
    return i + 1;
}
int numberPasses = 0; 
/*Perform randomized quick sort*/ 
void quicksort(unsigned long arr[], int p, int q)
{
    int j;

     numberPasses++;
    if (p < q)
    {
        j = partion(arr, p, q);
        quicksort(arr, p, j-1);
        quicksort(arr, j+1, q);
        
    }

   
  
}


int main(int argc, char *argv[])
{
    printf("\nRunning Base line program\n");
    printf("-----------------------------\n");
    printf("Randomized quick sort\n");


    clock_t start, end, duration;
  
    start = clock();
    FILE * inputFile;
    FILE * file_output;
    int counter = 0;
    unsigned long numRecords;

    int k;
    unsigned long num;

    int sort_order;

    /*Initializing an array to read the input file*/
    unsigned long *arr; 
    /* Reading input file */
    FILE *file_input = fopen(argv[1], "r");
    

    if (file_input == NULL)
    {
        fprintf(stderr, "Could not open input file 'extsort.input' \n");
        
        exit(1);
    }


    while(  fscanf(file_input, "%016lld", &num)>0)
     {
     
            numRecords++;
     }
     fclose(file_input);

    
    arr =  (unsigned long *)malloc(numRecords * sizeof(unsigned long));


    file_input = fopen(argv[1], "r");
    int i=0;
   
    while (!feof(file_input))
    {

        while(fscanf(file_input, "%016lld", &num) > 0) {
            arr[i] = num;
            i++;    
     }
        
    }
  
    sort_order = atoi(argv[3]); 

    //random_shuffle(arr, numRecords);
    quicksort(arr, 0, i-1); //function to sort the elements of array
    file_output = fopen(argv[2], "w");

    if (file_output == NULL)
     {
        printf("Error writting to file\n");
        exit(1);
     }

    
    if(sort_order==1)
        {
            for (k = i-1; k >= 0; k--)
            {
                fprintf(file_output,"%016lld \n", arr[k]);
            }          
                
            
        }

    else if(sort_order==0)
    {   // increasing/ decreasing order : 0 - increasing 1 - decreasing
      
        for (k = 0; k < i; k++)
            {
                fprintf(file_output,"%016lld \n", arr[k]);

            }       

    }  


      
     fclose(file_output);

     end = clock();
     duration = (double)(end - start)/CLOCKS_PER_SEC;


     printf("\nTotal number of passes required for randomized quick sort: %d\n", numberPasses);
     printf("Total execution time for randomized quick sort: %fsec", duration);

    return 0;
}