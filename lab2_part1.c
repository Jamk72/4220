#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 200 // NUM_THREAD # to fill in the arrays I made temporarily. 

//a struct was made because pthread function can only take in one argument.
//as shown below there are multiple variable in the struct that needed to be passed.
typedef struct{
    int matrix[100][100];
    int rowNum = 0;
    int colNum = 0;
    int rowNum2 = 0;
    int colNum2 = 0;
    int int1 = 0;
    int int2 = 0;
    int int3 = 0;
}mainMatrix;  

//array of threads for the multiple threads that will be used in the row and element functions
mainMatrix mainMatrix_array[NUM_THREADS]; 

// pthread function prototypes
void* matrixThread(void* convolveMatrix);
void* rowThread(void* convolveMatrix);
void* elementThread(void* convolveElement);


///////////MAIN FUNCTION/////////////
int main(int argc, char *argv[]){
    //decleration of the clock.
    clock_t start[3];
    clock_t end[3];
    double endTime[3];

    //executable argument check
    if(argc != 2){
        printf("Could not execute\n");
        return -1;
    }

    //opening file to read
    FILE *filePtr;
    filePtr = fopen(argv[1], "r");

    //file open check
    if(!filePtr){
        printf("Could not execute file\n");
        return -1;
    }
    
    // struct variable.
    mainMatrix matrix1;

    //reading from the file and storing it into struct variables.
    fscanf(filePtr, "%d %d", &matrix1.rowNum, &matrix1.colNum);

    printf("Row: %d Column: %d\n", matrix1.rowNum, matrix1.colNum);

    //reading each element from the file.
    for(int i = 0; i < matrix1.rowNum; i++){
        for(int j = 0; j < matrix1.colNum; j++){
            fscanf(filePtr, "%d", &matrix1.matrix[i][j]);
        }
    }

    for(int i = 0; i < matrix1.rowNum; i++){
        for(int j = 0; j < matrix1.colNum; j++){
            printf("%d ", matrix1.matrix[i][j]);
        }
        printf("\n");
    }

    // reading the filters from the element and printing them
    fscanf(filePtr, "%d %d", &matrix1.rowNum2, &matrix1.colNum2);

    printf("Row: %d Column: %d\n", matrix1.rowNum2, matrix1.colNum2);

    fscanf(filePtr, "%d %d %d", &matrix1.int1, &matrix1.int2, &matrix1.int3);

    printf("%d %d %d", matrix1.int1, matrix1.int2, matrix1.int3);

    printf("\n\n");

    //////////PTHREAD FUNCTION #1/////////////

    //convolve(matrix1);
    int t;
    int rc;
    int counter = 0;

    start[1] = clock();

    pthread_t thread1;
    //create thread w/ thread check
    rc = pthread_create(&thread1, NULL, matrixThread, (void*) &matrix1); 
    if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
    }
    //waiting for thread to end to continue to the next. 
    rc = pthread_join(thread1, NULL);

    //end clock and find total time of execution for thread 1.
    end[1] = clock();
    endTime[1] = (double)(end[1] - start[1]) / CLOCKS_PER_SEC;
    counter++;

    printf("\n\n");

    ////////////////PTHREAD FUNCTION #2/////////////
    start[2] = clock(); //start clock

    //declaring a thread array.
    pthread_t thread2[NUM_THREADS];

    //creating a thread for each row in the file as well as waiting for it to end with join.
    for(int k = 0; k < matrix1.rowNum; k++){
        mainMatrix_array[k] = matrix1;
        
        rc = pthread_create(&thread2[k], NULL, rowThread, &mainMatrix_array[k]);
        rc = pthread_join(thread2[k], NULL);
        counter++;

        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    //end clock and find total time of execution for thread 2.
    end[2] = clock();
    endTime[2] = (double)(end[2] - start[2]) / CLOCKS_PER_SEC;
    printf("\n\n");

    //////////////PTHREAD FUNCTION #3//////////////
    start[3] = clock(); //start clock

    //creating a double array thread for each element in the file.
    pthread_t thread3[NUM_THREADS][NUM_THREADS];

    //creating a thread for each element in the file as well as waiting for it to end with join.
    for(int k = 0; k < matrix1.rowNum; k++){
        for(int l = 0; l < matrix1.colNum; l ++){

        rc = pthread_create(&thread3[k][l], NULL, elementThread, &mainMatrix_array[k]);
        rc = pthread_join(thread3[k][l], NULL);
        counter++;

        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        }
    }

    //end clock and find total time of execution for thread 3.
    end[3] = clock();
    endTime[3] = (double)(end[3] - start[3]) / CLOCKS_PER_SEC;
    printf("Time for Thread 1 of the whole Matrix is: %lf seconds\n", endTime[1]);
        printf("Time for Thread 2 of the whole Matrix is: %lf seconds\n", endTime[2]);
    printf("Time for Thread 3 of the whole Matrix is: %lf seconds", endTime[3]);
    printf("\n\n");
    printf("Total number of threads: %d", counter);
    printf("\n\n");

    //close the file
    fclose(filePtr);

    //exiting all pthreads
    pthread_exit(NULL);
}

///////MATRIX PTHREAD/////////
void* matrixThread(void* convolveMatrix){
    //creating a new struct pointer called temp
    mainMatrix *temp;

    //type casting the arg pointer as a struct pointer and equalling it to temp
    temp = (mainMatrix*) convolveMatrix;

    //new struct to hold the variables and print them out
    mainMatrix newM;
    
    //setting the temp variables in to local variables for easier read
    int row = temp->rowNum;
    int col = temp->colNum;
    int int1 = temp->int1;
    int int2 = temp->int2;
    int int3 = temp->int3;

    //convolving the matrix
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if(temp->matrix[i][col] == 0){ 
                newM.matrix[i][j] = (temp->matrix[i][j] * int2) + (temp->matrix[i][j+1] * int3);
            }
            if(temp->matrix[i][col - 1] == 0){ 
                newM.matrix[i][j] = (temp->matrix[i][j-1] * int1) + (temp->matrix[i][j] * int2);
            }
            else{
                newM.matrix[i][j] = (temp->matrix[i][j-1] * int1) + (temp->matrix[i][j] * int2) + (temp->matrix[i][j+1] * int3);
            }
        }
    }

    printf("Below is the matrix pthread\n");

    //printing out the matrix
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            printf("%d ", newM.matrix[i][j]);
        }
        printf("\n");
    }

    //exiting the thread
    pthread_exit(NULL);
} 

////////ROW PTHREAD/////////

void* rowThread(void* convolveRow){
    mainMatrix *temp;

    temp = (mainMatrix*) convolveRow;

    mainMatrix newM;

    int row = temp->rowNum;
    int col = temp->colNum;
    int int1 = temp->int1;
    int int2 = temp->int2;
    int int3 = temp->int3;

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if(temp->matrix[i][col] == 0){
                newM.matrix[i][j] = (temp->matrix[i][j] * int2) + (temp->matrix[i][j+1] * int3);
            }
            if(temp->matrix[i][col - 1] == 0){ 
                newM.matrix[i][j] = (temp->matrix[i][j-1] * int1) + (temp->matrix[i][j] * int2);
            }
            else{
                newM.matrix[i][j] = (temp->matrix[i][j-1] * int1) + (temp->matrix[i][j] * int2) + (temp->matrix[i][j+1] * int3);
            }
        }
    }

    
    printf("Below is the row pthread:\n");

    for(int i =0; i < row; i++){
        for(int j = 0; j < col; j++){
            printf("%d ", newM.matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    pthread_exit(NULL);
}  


void* elementThread(void* convolveElement){
    mainMatrix *temp;

    temp = (mainMatrix*) convolveElement;

    mainMatrix newM;

    int row = temp->rowNum;
    int col = temp->colNum;
    int int1 = temp->int1;
    int int2 = temp->int2;
    int int3 = temp->int3;

 //   printf("int1 value: %d", int3);
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if(temp->matrix[i][col] == 0){
                newM.matrix[i][j] = (temp->matrix[i][j] * int2) + (temp->matrix[i][j+1] * int3);
            }
            if((temp->matrix[i][col - 1] - 1) == 0){ 
                newM.matrix[i][j] = (temp->matrix[i][j-1] * int1) + (temp->matrix[i][j] * int2);
            }
            else{
                newM.matrix[i][j] = (temp->matrix[i][j-1] * int1) + (temp->matrix[i][j] * int2) + (temp->matrix[i][j+1] * int3);
            }
        }
    }

    printf("Below is the element pthread:\n");

    for(int i =0; i < row; i++){
        for(int j = 0; j < col; j++){
            printf("%d ", newM.matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    pthread_exit(NULL);
}



///////////ORIGINAL FUNCTION FOR CONVOLVING////////////
/*void convolve(mainMatrix temp){

    mainMatrix newM;

    for(int i = 0; i < temp.rowNum; i++){
        for(int j = 0; j < temp.colNum; j++){
            if(j == 0){
                newM.matrix[i][j] = (temp.matrix[i][j] * temp.int2) + (temp.matrix[i][j] * temp.int3);
            }

            if(temp.matrix[i][temp.colNum - 1] == 0){ //could I just do (j-1) == 0 ?
                newM.matrix[i][j] = (temp.matrix[i][j] * temp.int1) + (temp.matrix[i][j] * temp.int2);
            }
            else{
                newM.matrix[i][j] = (temp.matrix[i][j] * temp.int1) + (temp.matrix[i][j] * temp.int2) + (temp.matrix[i][j] * temp.int3);
            }
        }
    }

    for(int i = 0; i < temp.rowNum; i++){
        for(int j = 0; j < temp.colNum; j++){
            printf("%d ", newM.matrix[i][j]);
        }
        printf("\n");
    }
        
}*/

