#include <iostream>
#include <algorithm>
#include <mpi.h>
#include "Process.cpp"

using namespace std;

int main(int argc, char **argv){
    int myRank, size; // ints for storing the current processes rank and the size of the COMM_WORLD communicator
    int left_rank, right_rank; // ints for storing neighbor's ranks
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size); // size of the world (number of total processes)
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank); // Current process's unique rank

    Process p(myRank);
    cout << "Process " << p.getRank() << " initialized!" << endl;
    
    // loop until leader is elected. First process to break out of loop will be elected leader
    while(true){
        if(p.isActive()) // If THIS process is active
        {
            if(myRank ==0) // If THIS is the first process
            {
                MPI_Send(&myRank, 1, MPI_INT, myRank+1, 0, MPI_COMM_WORLD); // send myrank to the left neighbor
                MPI_Recv(&right_rank, 1, MPI_INT, size-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the rank of the right neighbor

                MPI_Send(&myRank, 1, MPI_INT, size-1, 0, MPI_COMM_WORLD); // send myrank to the right neighbor
                MPI_Recv(&left_rank, 1, MPI_INT, myRank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receive the rank from the left neighbor
            }
            else if(myRank == size-1) // If THIS is the last process
            {
                MPI_Send(&myRank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // send myrank to the left neighbor
                MPI_Recv(&right_rank, 1, MPI_INT, myRank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the rank of the right neighbor

                MPI_Send(&myRank, 1, MPI_INT, myRank-1, 0, MPI_COMM_WORLD); // send myrank to the right neighbor
                MPI_Recv(&left_rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receive the rank from the left neighbor
            }
            else // all other processes
            {
                MPI_Send(&myRank, 1, MPI_INT, myRank+1, 0, MPI_COMM_WORLD); // send myrank to the left neighbor
                MPI_Recv(&right_rank, 1, MPI_INT, myRank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the rank of the right neighbor

                MPI_Send(&myRank, 1, MPI_INT, myRank-1, 0, MPI_COMM_WORLD); // send myrank to the right neighbor
                MPI_Recv(&left_rank, 1, MPI_INT, myRank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receive the rank from the left neighbor
            }

            if(myRank < min(left_rank, right_rank)) // If my rank is smaller than both my neighbors
            {
                p.setActive(false); // set ourself to passive
            }
            if(myRank == left_rank && myRank==right_rank) // if my left neighbor and right neighbor are the same as me
            {
                break; // this means all other processes are passive and I can break out of the loop and elect myself leader
            }
        }
        else // if THIS process is passive, we will be relaying our neighbors ranks to the other neighbor. myRank is not longer relevant because I am passive
        {
            // NOTE that you must receive before you can send to avoid deadlocks
            if (myRank == 0)
            {
                MPI_Recv(&right_rank, 1, MPI_INT, size-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the rank of the right neighbor
                MPI_Send(&right_rank, 1, MPI_INT, myRank+1, 0, MPI_COMM_WORLD); // relay right rank to left neighbor

                MPI_Recv(&right_rank, 1, MPI_INT, myRank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the rank of the right neighbor
                MPI_Send(&right_rank, 1, MPI_INT, size-1, 0, MPI_COMM_WORLD); // relay right rank to left neighbor
            }
            else if (myRank == size-1){
                MPI_Recv(&right_rank, 1, MPI_INT, myRank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the rank of the right neighbor
                MPI_Send(&right_rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // relay right rank to left neighbor

                MPI_Recv(&right_rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the rank of the right neighbor
                MPI_Send(&right_rank, 1, MPI_INT, myRank-1, 0, MPI_COMM_WORLD); // relay right rank to left neighbor
            }
            else
            {
                MPI_Recv(&right_rank, 1, MPI_INT, myRank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the rank of the right neighbor
                MPI_Send(&right_rank, 1, MPI_INT, myRank+1, 0, MPI_COMM_WORLD); // relay right rank to left neighbor

                MPI_Recv(&right_rank, 1, MPI_INT, myRank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // receive the rank of the right neighbor
                MPI_Send(&right_rank, 1, MPI_INT, myRank-1, 0, MPI_COMM_WORLD); // relay right rank to left neighbor
            }            
        }
    }

    // If THIS process breaks out of the loop then it is the leader. We will elect ourself as the leader
    cout << "I am Process " << myRank << " and I am the elected leader!" << endl;

    MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS); // Broadcast a message to all other processes to terminate them
    MPI_Finalize(); // Finalize and exit MPI
    return 0;
}