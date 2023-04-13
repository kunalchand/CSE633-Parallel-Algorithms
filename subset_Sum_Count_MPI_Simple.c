#include <mpi.h>
#include <stdio.h>

#define ROWS 5
#define COLS 1

int main(int argc, char** argv) {
    int rank, size;
    int input[ROWS-1] = {3,3,3,3};
    int sum = 6;
    int memory[ROWS][COLS];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int i, j;

    // Initialize
    if(rank == 0){
      for(i=0; i<ROWS; i++){
	    memory[i][0] = 1;
      }
    }
    else{
      memory[0][0]=0;
    }
    
    // Row Wise Iteration
    for(i=1; i<ROWS; i++){
      
      // Send Phase
      if(rank < size - input[i-1]){
	    MPI_Send(&memory[i-1][0], COLS, 
	                MPI_INT, rank+input[i-1], 
	                i, MPI_COMM_WORLD); 
      }

      // Receive Phase
      int fetchedValue;
      if(rank == input[i-1] || rank > input[i-1]){
	    MPI_Recv(&fetchedValue, 
	                COLS, MPI_INT, rank-input[i-1], 
	                MPI_ANY_TAG, MPI_COMM_WORLD, 
	                MPI_STATUS_IGNORE);
      }

      // Compute Phase
      if(input[i-1] > rank)
	    memory[i][0] = memory[i-1][0];
      else
	    memory[i][0] = memory[i-1][0] + fetchedValue;
	    
	  MPI_Barrier(MPI_COMM_WORLD);
    }

    // Print Final Answer:
    if(rank == size-1)
      printf("Subset Sum Count: %d\n",memory[ROWS-1][0]);

    MPI_Barrier(MPI_COMM_WORLD);

    // Print Memory Array:
    // printf("Rank %d Memory Array:\n",rank); 
    // for(i=0; i<ROWS; i++){
    // printf("%d\n",memory[i][0]);
    // }

    MPI_Finalize();
    return 0;
}