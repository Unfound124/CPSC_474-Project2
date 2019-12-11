# CPSC_474-Project2

by Vijay Duggirala
CWID 802302208
Fall 2019

Franklin's leader election algorithm for Ring topology

To run the program simply open a bash terminal and run ./start.sh


While inside the container, to compile the program:
```
make
```

To run the program with some preset number of processes:
```
make run-1
```
or 
```
make run-4
```

or 
```
make run-10
```

In general:
```
mpirun -n [N] ./main
```
where [N] is the number of processes to spawn
