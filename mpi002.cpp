#include <stdio.h>
#include <mpi.h>

int main(int c, char **v){
  MPI_Init(&c, &v);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int send_value = rank;
  int recv_value = -3;

  const int TAG = 1;
  MPI_Status st;

  if(rank == 0){
    MPI_Send(&send_value, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
  }else if(rank == 1){
    MPI_Recv(&recv_value, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &st);
  }

  printf("rank = %d : rv = %d\n", rank, recv_value);
  MPI_Finalize();
}
