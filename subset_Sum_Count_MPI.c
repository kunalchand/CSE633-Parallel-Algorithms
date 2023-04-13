#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define ROWS 66
#define SUM 2000
int input[ROWS-2] = {4, 22, 119, 94, 59, 19, 98, 99, 58, 142, 75, 19, 98, 106, 85, 135, 59, 127, 71, 131, 20, 123, 124, 30, 100, 121, 57, 76, 73, 96, 22, 110, 104, 128, 83, 2, 138, 6, 47, 114, 91, 147, 25, 134, 5, 148, 136, 90, 2, 83, 32, 120, 28, 38, 111, 82, 28, 18, 100, 109, 1, 80, 23, 11};
// Expected Answer (confirmed from GFG): 52221911
// Range of Number of Processors = 1 to 250
// GFG Sequential Version: https://www.geeksforgeeks.org/count-of-subsets-with-sum-equal-to-x/#

#define COL_PER_NODE 1 // Minimum number of columns per processor

void print(char string[]){
    printf("%s", string);
}

void printArray(int* array, int size) {
    int i;
    printf("[");
    for (i = 0; i < size; i++) {
        printf("%d", array[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main(int argc, char** argv) {

    unsigned long long usec;
    struct timeval tstart, tend;

    int rank, size;
    int COLS = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if(COL_PER_NODE*size - 1 >= SUM){
        COLS = COL_PER_NODE;
    }
    else {
        int result = SUM+1;
        while (result % size != 0){
            result += 1;
        }
        COLS = result/size;
    }
    
    int memory[ROWS][COLS];
    
    int i, j;

//===========================================INITIALISATION===========================================    

    // Initialize 0th Row
    for(j=0; j<COLS; j++){
        memory[0][j] = j + COLS*rank;
    }

    // Initialize 1st Row
    if(rank == 0){
        for(j=0; j<COLS; j++){
            memory[1][j] = (j == 0)?1:0;
        }
    }
    else{
        for(j=0; j<COLS; j++){
	        memory[1][j] = 0;
	    }
    }
    
    // Initialize Remaining Rows
    for(i=2; i<ROWS; i++){
        for(j=0; j<COLS; j++){
            memory[i][j] = -1;
        }
    }

//=============================================EXECUTION=============================================    
    gettimeofday(&tstart, NULL);

    // Row Wise Iteration
    for(i=2; i<ROWS; i++){
        
        // Send Phase ...............................................................................  
        int destination_Rank1 = -1;
        int destination_Rank2 = -1;
        
        int sendBufferSize1 = 0;
        int sendBufferSize2 = 0;
        
        for(j=0; j<COLS; j++){
            if(memory[0][j] < size*COLS - input[i-2]){
                int destination = memory[0][j]+input[i-2];
                int factor = COLS;
                int destination_Rank = destination/factor;
                if(destination_Rank1 == -1 || destination_Rank1 == destination_Rank){
                    destination_Rank1 = destination_Rank;
                    sendBufferSize1++;
                }
                else{
                    destination_Rank2 = destination_Rank;
                    sendBufferSize2++;
                }
            }
        }
        
        int* firstSendBuffer = malloc(sendBufferSize1 * sizeof(int));
        int* secondSendBuffer = malloc(sendBufferSize2 * sizeof(int));
        
        int sizeFlag = 0;
        int bufferFlag = 1;
        for(j=0; j<COLS; j++){
            if(bufferFlag == 1){
                if(sizeFlag < sendBufferSize1){
                    firstSendBuffer[sizeFlag] = memory[i-1][j];
                    sizeFlag++;
                    if(sizeFlag == sendBufferSize1){
                        sizeFlag = 0;
                        bufferFlag = 2;
                    }
                }
            }
            else if(bufferFlag == 2){
                if(sizeFlag < sendBufferSize2){
                    secondSendBuffer[sizeFlag] = memory[i-1][j];
                    sizeFlag++;
                }
            }
        }
        
        if(destination_Rank1 != -1){
            MPI_Send(firstSendBuffer, sendBufferSize1, MPI_INT, destination_Rank1, 1, MPI_COMM_WORLD);
        }
        
        if(destination_Rank2 != -1){
            MPI_Send(secondSendBuffer, sendBufferSize2, MPI_INT, destination_Rank2, 2, MPI_COMM_WORLD); 
        }
        
        free(firstSendBuffer);
        free(secondSendBuffer);
        
        // MPI_Barrier(MPI_COMM_WORLD);

        // Receive Phase ..........................................................................
        int source_Rank1 = -1;
        int source_Rank2 = -1;
        
        int recieveBufferSize1 = 0;
        int recieveBufferSize2 = 0;
        
        for(j=COLS-1; j>=0; j--){
            if(memory[0][j] == input[i-2] || memory[0][j] > input[i-2]){
                int source = memory[0][j]-input[i-2];
                int factor = COLS;
                int source_Rank = source/factor;
                if(source_Rank1 == -1 || source_Rank1 == source_Rank){
                    source_Rank1 = source_Rank;
                    recieveBufferSize1++;
                }
                else{
                    source_Rank2 = source_Rank;
                    recieveBufferSize2++;
                }
            }
        }
        
        int* firstRecieveBuffer = malloc(recieveBufferSize1 * sizeof(int));
        int* secondRecieveBuffer = malloc(recieveBufferSize2 * sizeof(int));
        
        if(source_Rank1 != -1){
            MPI_Recv(firstRecieveBuffer, recieveBufferSize1, MPI_INT, source_Rank1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);   
        }
        
        if(source_Rank2 != -1){
            MPI_Recv(secondRecieveBuffer, recieveBufferSize2, MPI_INT, source_Rank2, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);   
        } 
        
        // MPI_Barrier(MPI_COMM_WORLD);

        // Compute Phase ...........................................................................
        for(j=0; j<COLS; j++){
            memory[i][j] = memory[i-1][j];
        }
        
        int firstBufferPointer = -1;
        int secondBufferPointer = -1;
        if(recieveBufferSize1 > 0){
            firstBufferPointer = recieveBufferSize1 - 1;
        }
        if(recieveBufferSize2 > 0){
            secondBufferPointer = recieveBufferSize2 - 1;
        }
        
        for(j=COLS-1; j>=0; j--){
            if(input[i-2] < memory[0][j] || input[i-2] == memory[0][j]){
                if(firstBufferPointer > -1){
                    memory[i][j] += firstRecieveBuffer[firstBufferPointer];
                    firstBufferPointer--;
                }
                else if(secondBufferPointer > -1){
                    memory[i][j] += secondRecieveBuffer[secondBufferPointer];
                    secondBufferPointer--;
                }
            }
        }
        
        free(firstRecieveBuffer);
        free(secondRecieveBuffer);
        
        MPI_Barrier(MPI_COMM_WORLD);
    }
    
//===============================================PRINT===============================================
    
    //  MPI_Barrier(MPI_COMM_WORLD);
    
    int answer_Rank = SUM / COLS;
    if(rank == answer_Rank){
        for(j=0; j<COLS; j++){
            // Print Final Answer:
            if(memory[0][j] == SUM){
                print("\nInput :");
                printArray(input, ROWS-2);
                printf("\nTarget Sum: %d\n", SUM);
		        printf("\nNumber of rows in each processor: %d\n",ROWS);
		        printf("Number of processors: %d\n", size);
                printf("Number of columns used per processor: %d\n", COLS);
                printf("2D Matrix Size: %d x (%d x %d) = %d x %d\n", ROWS,size,COLS,ROWS,size*COLS);
                printf("\nSubset Sum Count: %d\n\n",memory[ROWS-1][j]);

                gettimeofday(&tend, NULL);

                if (tend.tv_usec > tstart.tv_usec){
                    usec = (tend.tv_sec - tstart.tv_sec) * 1000000 + tend.tv_usec - tstart.tv_usec;
                } 
                else{
                    usec = (tend.tv_sec - (tstart.tv_sec + 1)) * 1000000 + (1000000 + tend.tv_usec - tstart.tv_usec);
                }

                printf( "Finished in %f seconds.\n\n",(double)usec/1000000.0);
            }
        }
    }
      

    // MPI_Barrier(MPI_COMM_WORLD);

    // Print Memory Array:
    // printf("Rank %d Memory Array:\n",rank); 
    // for(i=0; i<ROWS; i++){
    //     for(j=0; j<COLS; j++){
    //         printf("%d ",memory[i][j]);
    //     }
    //     printf("\n");
    // }

    MPI_Finalize();
    return 0;
}
