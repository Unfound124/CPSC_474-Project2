PROGRAM=main
MPICC=mpic++
FLAGS=-std=c++11 -g -Wall

all: ${PROGRAM}

${PROGRAM}: main.cpp
	${MPICC} ${FLAGS} -o ${PROGRAM} main.cpp

run-1: ${PROGRAM}
	clear
	mpirun -n 1 ./main

run-4: ${PROGRAM}
	clear
	mpirun -n 4 ./main

run-10: ${PROGRAM}
	clear
	mpirun -n 10 ./main

run-20: ${PROGRAM}
	clear
	mpirun -n 20 ./main

run-50: ${PROGRAM}
	clear
	mpirun -n 50 ./main
clean:
	rm ${PROGRAM}
