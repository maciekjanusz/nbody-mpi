EXECS=nbody-mpi
MPICC?=mpicc

all: ${EXECS}

nbody-mpi: nbody-mpi.c
	${MPICC} -o nbody-mpi nbody-mpi.c

clean:
	rm ${EXECS}

run:
	mpirun -np 4 -machinefile nodes $(PWD)/${EXECS}
