#include <stdio.h>

int findtheindex(int *array,int start_index,int end_index);

int main(int argc, char *argv[])
{

int counter;							// This variable is used for reading the file and loading in the array.
int array_size;
int input_value;						// This variable is used to store the first input of the file.
int i,flag=0;							// This variable is uded in the FOR loop for reading the command line argument

FILE *fp;							// Declaring file pointer
int my_array[1000000];						// Declaring the array with a random size. In this array the inputs from the files will be loaded


	if (argc == 2)
	{
		counter=0;										
		fp = fopen(argv[1],"r");			// Opening file pointer for the filename received from command line
	while (fscanf(fp,"%d",&input_value)!=EOF)		// Starts to read the file
        {
                if(counter == 0)				// Considering the first input of the file 
                {
                array_size=input_value+1;				// First input of the file is taken as the array size
                #define size array_size				// Define constant "size" with the value of first input of the file
                int my_array[size];				// Declaring the array again. Now it has the same number of cell as the same no of inputs the file has
                counter++;
                }
                else {
                my_array[counter]=input_value;		// Populating the array with the help of the counter variable
                counter++;
		}
        }
	fclose(fp);						// Array is populated and the file is being closed
	}

	my_array[0] = size - 1;

	int mohsin = 0;

	mohsin = findtheindex(&my_array[0],0,size-1);

	if(mohsin == -1)
		printf("No index i for which A[i]=i\n");
	else
		printf("A[%d] = %d\n",mohsin,my_array[mohsin]);
	
	
	return 0;
}


int findtheindex(int *array,int start_index,int end_index)
{

int i;

	if(start_index > end_index)
	{
		return -1;
		
	}

	if(array[(start_index + end_index)/2] == (start_index + end_index)/2)
	{
		return (start_index + end_index)/2;
	}
	else if(array[(start_index + end_index)/2] > (start_index + end_index)/2)
	{
		end_index = (start_index + end_index)/2 - 1;
		i = findtheindex(array,start_index,end_index);
		return i;
		
	}
	else if(array[(start_index + end_index)/2] < (start_index + end_index)/2)
	{
		start_index = (start_index + end_index)/2 + 1;
		i = findtheindex(array,start_index,end_index);
		return i;
	
	}

}
