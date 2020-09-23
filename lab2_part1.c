//#include <pthreads.h>
#include <stdio.h>
#include <stdlib.h>






int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Could not execute\n");
        return -1;
    }

    FILE *filePtr;
    filePtr = fopen(argv[1], "r");

    if(!filePtr){
        printf("Could not execute file\n");
        return -1;
    }
    
    int rowNum = 0;
    int colNum = 0;
    int rowNum2 = 0;
    int colNum2 = 0;

    fscanf(filePtr, "%d %d", &rowNum, &colNum);

    printf("Row: %d Column: %d\n", rowNum, colNum);

    int matrix[rowNum][colNum];

    for(int i = 0; i < rowNum; i++){
        for(int j = 0; j < colNum; j++){
            fscanf(filePtr, "%d", &matrix[i][j]);
        }
    }

    for(int i = 0; i < rowNum; i++){
        for(int j = 0; j < colNum; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    // printing out the filters
    fscanf(filePtr, "%d %d", &rowNum2, &colNum2);

    printf("Row: %d Column: %d\n", rowNum2, colNum2);

    int matrix2[rowNum2][colNum2];

    for(int i = 0; i < rowNum2; i++){
        for(int j = 0; j < colNum2; j++){
            fscanf(filePtr, "%d", &matrix2[i][j]);
        }
    }

    for(int i = 0; i < rowNum2; i++){
        for(int j = 0; j < colNum2; j ++){
            printf("%d ", matrix2[i][j]);
        }
        printf("\n");
    }



    fclose(filePtr);
}

//void* matrixTh(){}

//void* rowTh(){}

//void* elementTh(){}
