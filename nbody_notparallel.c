#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
//#include <mpi.h>

#define WIDTH  1000
#define HEIGHT 1000
#define BODIES 11

typedef struct Particle{
	float x, y, xnew, ynew; // wspolrzedne
	float m;		// masa
	float vx, vy;  		// predkosc
	float fx, fy;  		// sila
	float ax, ay;  		// przyspieszenie
	struct Particle *next;   
        struct Particle *prev; 
} Particle;

void ComputeForces(Particle *particles){
int i, j;
float dx, dy, d;
float G = 0.002;
float F, dt =1; 
	for (i = 0; i < BODIES; i++) {
			for (j=0 ;j< BODIES;j++){
				if(i!=j){
					dx = particles[i].x - particles[j].x;
					dy = particles[i].y - particles[j].y;
					d = sqrt(dx*dx+dy*dy);
					F = (G*particles[i].m*particles[j].m)/(d*d);
					F = (G*particles[i].m*particles[j].m)/(d*d);
					particles[i].fx += F * dx/d;
					particles[i].fy += F * dy/d;
				}
			}
			particles[i].vx += dt*particles[i].fx/particles[i].m;
     			particles[i].vy += dt*particles[i].fy/particles[i].m;
			particles[i].x += dt * particles[i].vx;
     			particles[i].y += dt * particles[i].vy;	
	}
	
}

void update(Particle *particles){
	

}

void WriteParticles(Particle *particles){
	int i=0;
	FILE *fp;
	if ((fp=fopen("out.txt", "w"))==NULL) {
     		printf ("Nie mogę otworzyć pliku\n");
     		exit(1);
     	}
	while(i != BODIES){
   		fprintf (fp, "%f\t%f\t%f\n", particles[i].x, particles[i].y, particles[i].m);
		i++;
	}
   	fclose (fp);
}


void PrintParticles(Particle *particles){
	int i;
	for (i=0; i<BODIES; i++) {
	    printf( "%f \t %f \t %f\n", particles[i].x, particles[i].y, particles[i].m);
	}
}

int main(int argc, char* argv[]) {
    int size, rank, i=0;
    float x, y, m;
    Particle  *recvbuf;
    //MPI_Init(&argc, &argv);
    //MPI_Comm_size(MPI_COMM_WORLD, &size);
    //MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
    Particle particles[BODIES];
	// Pobiera dane
		i=0;
		FILE *fp;
		if ((fp=fopen("data.txt", "r"))==NULL) {
     			printf ("Nie mogę otworzyć pliku\n");
     		exit(1);
    		}
		while(fscanf(fp, "%f %f %f", &x, &y, &m) != EOF){
			particles[i].x =x;
			particles[i].y =y;
			particles[i].m =m;
			i++;
		}
		fclose(fp);
	// Rozsyla dane poczatkowe, czyli polozenie i mase
	/*
	for (i = 0; i < BODIES; i++) {
		MPI_Bcast(&particles[i].x, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Bcast(&particles[i].y, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Bcast(&particles[i].m, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	}*/
int k;
//if (rank == 0) {
	for(k=0 ; k<15; k++){
        PrintParticles(particles);
	printf("kolejna porcja danych\n");
	ComputeForces(particles);
	PrintParticles(particles);
	}
//}


  
//MPI_Finalize();
return 0;
}
