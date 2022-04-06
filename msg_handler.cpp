#include "msg_handler.hpp"

#include <mpi.h>

#define ACK_FOR_K 100
#define REQ_FOR_K 101

#define ACK_FOR_M 102
#define REQ_FOR_M 103

void msg_handler(const int & _rank, DockManager & manager)
{
    printf("Watek komunikacyjny START!\n");

    const int myProcRank{_rank};
    int come_lamp_value;
    MPI_Status status;
    
    while (true)
    {
        MPI_Recv(&come_lamp_value, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if(status.MPI_SOURCE == myProcRank)
        {
            if(status.MPI_TAG == REQ_FOR_K)
            {

            }
            else if(status.MPI_TAG == REQ_FOR_M)
            {

            }
        }
        else
        {
            if(status.MPI_TAG == ACK_FOR_K)
            {

            }
            else if(status.MPI_TAG == ACK_FOR_M)
            {

            }
            else if(status.MPI_TAG == REQ_FOR_K)
            {
                manager.addProcesToDockQueue({status.MPI_SOURCE, come_lamp_value});
            }
            else if(status.MPI_TAG == REQ_FOR_M)
            {
                manager.addProcesToMechanicQueue({status.MPI_SOURCE, come_lamp_value});
            }

        }
    }
}