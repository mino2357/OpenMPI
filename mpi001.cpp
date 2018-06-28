#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double myrand(){
  return (double) rand() / (double) RAND_MAX;
}

double calc_pi(int seed, int trial){
  srand(seed);
  int n = 0;
  for(int i=0; i<trial; ++i){
    double x = myrand();
    double y = myrand();
    if( x*x + y*y < 1.0){
      n++;
    }
  }
  return 4.0 * (double) n / (double) trial;
}

int main(int c, char **v){
  MPI_Init(&c, &v);
  
  int rank;
  int procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  
  double pi = calc_pi(rank, 10000000);
  printf("rank = %d:/%d %.14f\n", rank, procs, pi);

  MPI_Barrier(MPI_COMM_WORLD);

  double sum = 0;

  MPI_Allreduce(&pi, &sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  sum = sum / (double) procs;

  if(rank == 0){
    printf("average = %.14f\n", sum);
  }
  
  MPI_Finalize();
}
