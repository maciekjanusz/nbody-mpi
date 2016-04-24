#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ROOT 0

const double G = 0.002;
const double dt = 1;

int count_lines(FILE* fp) {
  rewind(fp);
  char ch;
  int lines = 0;
  while(!feof(fp)) {
    ch = fgetc(fp);
    if(ch == '\n') {
      lines++;
    }
  }
  return lines;
}

void read_particles(FILE* fp, double* x, double* y,
  double* m, double* vx, double* vy) {
  rewind(fp);
  int i = 0;
  while(fscanf(fp, "%lf %lf %lf",x+i, y+i, m+i) != EOF) {
    i++;
  }
}

void print_particles(int N, double* x, double* y,
  double* m, double* vx, double* vy) {
  for(int i = 0; i < N; i++) {
    printf("(%.0f, %.0f, %.0f, %.0f, %.0f) ", x[i], y[i], m[i], vx[i], vy[i]);
  }
  printf("\n");
}

int main(int argc, char *argv[]) {

  // DECLARATIONS
  int rank, comm_size; // communicator
  int i, j, N;
  int t_steps = 0;
  int t_max = 10; // max time steps
  double dt; // time step value
  FILE* file;

  // INIT
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  if(rank == ROOT) {
    // on root node, get particles count
    file = fopen("data.txt", "r");
    N = count_lines(file);
  }

  // send N (particles count) to all processes
  MPI_Bcast(&N, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
  // calculate data chunk size per process
  const int chunk_size = ceil((double) N / comm_size);
  const int chunk_start = rank * chunk_size;

  // allocate arrays:
  // mass of particles, broadcast at start, keep constant
  double* m = (double*) malloc(N * sizeof(double));
  // coordinates, broadcast at start, changes in each iteration
  double* x = (double*) malloc(N * sizeof(double));
  double* y = (double*) malloc(N * sizeof(double));
  // velocity, scatter at start, changes in each iteration, used locally
  double* vx = (double*) malloc(N * sizeof(double));
  double* vy = (double*) malloc(N * sizeof(double));
  // forces - used only locally
  double* fx = (double*) malloc(N * sizeof(double));
  double* fy = (double*) malloc(N * sizeof(double));

  if(rank == ROOT) {
    // on root node, read particles data from file
    read_particles(file, x, y, m, vx, vy);
    fclose(file);
  }

  // ---------------- START PROCESSING --------------------

  // broadcast mass to all processes. this will stay constant
  MPI_Bcast(m, N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
  // broadcast initial coordinates to all processes. those will change
  MPI_Bcast(x, N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
  MPI_Bcast(y, N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
  // broadcast initial velocity (better way is to use MPI_Scatter to save memory)
  MPI_Bcast(vx, N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
  MPI_Bcast(vy, N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

  // start with time 0 + dt;
  while(++t_steps <= t_max) {
    // portion for <rank> process
    for(i = chunk_start; i < chunk_start + chunk_size; i++) {
      // if i is greater than N, do not process
      if(i >= N) continue;
      for(j = 0; j < N; j++) {
        // omit this particle
        if(i == j) continue;
        // calculate force from each particle
        double dx = x[i] - x[j];
				double dy = y[i] - y[j];
				double d = sqrt(dx * dx + dy * dy);
				double F = (G * m[i] * m[j]) / (d * d);
				fx[i] += F * dx/d; // ---------------------- be careful, initialize to zeros
				fy[i] += F * dy/d; // jw.
      }
    }
    // forces are integrated. update position:
    for(i = chunk_start; i < chunk_start + chunk_size; i++) {
      vx[i] += dt * fx[i] / m[i];
      vy[i] += dt * fy[i] / m[i];
      x[i] += dt * vx[i];
      y[i] += dt * vy[i];
    }

    // 2. execute MPI_Allgather (in-place mode) to distribute results across all processes
    MPI_Allgather(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, // in-place mode
                  x, chunk_size, MPI_DOUBLE,
                  MPI_COMM_WORLD);
    MPI_Allgather(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, // in-place mode
                  y, chunk_size, MPI_DOUBLE,
                  MPI_COMM_WORLD);
    // at this point coord data is distributed and unified across all processes

    if(rank == ROOT) {
      print_particles(N, x, y, m, vx, vy);
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);
  free(m);
  free(x);
  free(y);
  free(vx);
  free(vy);
  free(fx);
  free(fy);

  // DEINIT
  MPI_Finalize();
}
