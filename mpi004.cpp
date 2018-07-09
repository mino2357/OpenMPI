#include <iostream>
#include <stdio.h>
#include <chrono>
#include <mpi.h>

void set_data(double *send, double *recv, int size, int rank){
  for(int i=0; i<size; ++i){
    send[i] = 1.0 + static_cast<double>(rank);
    recv[i] = 0.0;
  }
}

int main(int c, char **v){
  // mpi init
  int rank, num_procs;
  MPI_Status status;
  MPI_Init(&c, &v);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // error ctl
  if(num_procs != 2){
    std::cerr << "Number of processes must be 2." << std::endl;
    return 0;
  }

  // mem alloc
  double *send, *recv;
  int size = atoi(v[1]);
  send = new double[size];
  recv = new double[size];

  // set data
  std::cout << "Size : " << size << std::endl;
  set_data(send, recv, size, rank);
  MPI_Barrier(MPI_COMM_WORLD);

  // comm
  auto start = std::chrono::system_clock::now();
  if(rank == 0){
    MPI_Sendrecv(send, size, MPI_DOUBLE, 1, 0,
                 recv, size, MPI_DOUBLE, 1, 0,
                 MPI_COMM_WORLD, &status);
  }else{
    MPI_Sendrecv(send, size, MPI_DOUBLE, 0, 0,
                 recv, size, MPI_DOUBLE, 0, 0,
                 MPI_COMM_WORLD, &status);
  }
  auto stop = std::chrono::system_clock::now();

  // result
  auto diff = stop - start;
  std::cout << "[Rank " << rank << "] : send[0] = " << send[0] << ", recv[0] = " << recv[0] << std::endl;
  std::cout << "Elapsed Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << " [ms]" << std::endl;

  // mem free
  delete[] send;
  delete[] recv;
  MPI_Finalize();
}
