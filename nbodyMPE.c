#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mpe.h"
#include "mpe_graphics.h"
#include <string.h>

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
  while(fscanf(fp, "%lf %lf %lf %lf %lf",x+i, y+i, m+i, vx+i, vy+i) != EOF) {
    i++;
  }
}

void print_particles(int N, double* x, double* y,
  double* m, double* vx, double* vy) {
  for(int i = 0; i < N; i++) {
    printf("(%.0f, %.0f, %.0f, %.0f, %.0f)\n ", x[i], y[i], m[i], vx[i], vy[i]);
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  // DECLARATIONS;
  int rank, comm_size; // communicator
  int i, j, N, ierr;
  int t_steps = 0;
  int t_max = 40000; // max time steps
  int        b = 0, n = 0, button=0;
  FILE *file;
  int flag=0;
  const int  MOUSE_RIGHT_BUTTON = 3;
  MPE_XGraph canvas;
  // INIT
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  MPE_Open_graphics(&canvas, MPI_COMM_WORLD, NULL, -1, -1, 700, 700, 0);

  if(rank == ROOT) {
    // on root node, get particles count
    file = fopen("solar_system.txt", "r");
    N = count_lines(file);
  }

  // send N (particles count) to all processes
  MPI_Bcast(&N, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
  // calculate data chunk size per process
  const int chunk_size = ceil((double) N / comm_size);
  const int chunk_start = rank * chunk_size;
  const int buf_size = comm_size * chunk_size;

  // allocate arrays:
  // mass of particles, broadcast at start, keep constant
  double* m = (double*) malloc(buf_size * sizeof(double));
  // coordinates, broadcast at start, changes in each iteration
  double* x = (double*) malloc(buf_size * sizeof(double));
  double* y = (double*) malloc(buf_size * sizeof(double));
  // velocity, scatter at start, changes in each iteration, used locally
  double* vx = (double*) malloc(buf_size * sizeof(double));
  double* vy = (double*) malloc(buf_size * sizeof(double));
  // forces - used only locally
  double* fx = (double*) malloc(buf_size * sizeof(double));
  double* fy = (double*) malloc(buf_size * sizeof(double));

  if(rank == ROOT) for(i = 0; i < N; i++) {
    fx[i] = fy[i] = 0;
  }

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

  if(rank == ROOT) {
    print_particles(N, x, y, m, vx, vy);
  }
  MPE_Line_thickness( canvas, 4 );
  // start with time 0 + dt;
  while(++t_steps <= t_max) {
//MPE_Draw_point(canvas, (int)x, (int)y, MPE_GREEN);


    // portion for <rank> process
    for(i = chunk_start; i < chunk_start + chunk_size; i++) {
      // if i is greater than N, do not process
      if(i >= N) continue;
      for(j = 0; j < N; j++) {
        // omit this particle
        if(i == j) continue;
        // calculate force from each particle
        double dx = x[j] - x[i];
        double dy = y[j] - y[i];
	double d = sqrt(dx * dx + dy * dy);
        double F = (G * m[i] * m[j]) / (d * d);
        fx[i] += F * dx/d; // ---------------------- be careful, initialize to zeros
        fy[i] += F * dy/d; // jw.
      }
    }
    // forces are integrated. update position:
    for(i = chunk_start; i < chunk_start + chunk_size; i++) {
      if(i >= N) continue;
      vx[i] += dt * fx[i] / m[i];
      vy[i] += dt * fy[i] / m[i];
      x[i] += dt * vx[i];
      y[i] += dt * vy[i];
      // clear fx and fy
      fx[i] = fy[i] = 0;
      MPE_Fill_circle(canvas, (int)x[i], (int)y[i], 3, MPE_BLACK);
      MPE_Update(canvas);
    }

   
    // 2. execute MPI_Allgather (in-place mode) to distribute results across all processes
    MPI_Allgather(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, // in-place mode
                  x, chunk_size, MPI_DOUBLE,
                  MPI_COMM_WORLD);
    MPI_Allgather(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, // in-place mode
                  y, chunk_size, MPI_DOUBLE,
                  MPI_COMM_WORLD);
    // at this point coord data is distributed and unified across all processes

    // if(rank == ROOT) {
    //   print_particles(N, x, y, m, vx, vy);
    // }
	if(rank == ROOT) {
    		//write_particles(N, x, y, m, vx, vy, t_steps);
  	}
   
  }

  MPI_Barrier(MPI_COMM_WORLD);
  // DEINIT
  MPE_Close_graphics(&canvas);
  MPI_Finalize();

  free(m);
  free(x);
  free(y);
  free(vx);
  free(vy);
  free(fx);
  free(fy);
}
