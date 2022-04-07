#include "msg_handler.hpp"

#include <mpi.h>


void msg_handler(const int & _rank, const int & _size, DockManager & manager)
{
    printf("Watek komunikacyjny START %d!\n", _rank);

    const int myProcRank{_rank};
    const int procSize {_size};
    int come_lamp_value;
    MPI_Status status;
    
    while (true)
    {
        MPI_Recv(&come_lamp_value, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if(status.MPI_SOURCE == myProcRank)
        {
            if(status.MPI_TAG == REQ_FOR_K)
            {
                printf("%d ubiega sie o dostep do dokow\n", myProcRank);
                manager.addProcesToDockQueue({status.MPI_SOURCE, come_lamp_value});
                manager.printPrcessInDockQueue();
                for(int i = 0; i < procSize; ++i)
                {
                    if(i != myProcRank)
                    {
                        MPI_Send(&come_lamp_value , 1, MPI_INT , i , REQ_FOR_K , MPI_COMM_WORLD);
                    }
                }
            }
            else if(status.MPI_TAG == REQ_FOR_M)
            {
                printf("%d ubiega sie o dostep do mechanikow\n", myProcRank);
                manager.addProcesToMechanicQueue({status.MPI_SOURCE, come_lamp_value});
                manager.printProcessInMechanicQueue();
                for(int i = 0; i < procSize; ++i)
                {
                    if(i != myProcRank)
                    {
                        MPI_Send(&come_lamp_value , 1, MPI_INT , i , REQ_FOR_M , MPI_COMM_WORLD);
                    }
                }
            }
        }
        else
        {
            if(status.MPI_TAG == ACK_FOR_K)
            {
                printf("%d dostalem ACK K od %d\n", myProcRank, status.MPI_SOURCE);
                manager.incrementackK();
            }
            else if(status.MPI_TAG == ACK_FOR_M)
            {
                printf("%d dostalem ACK M od %d\n", myProcRank, status.MPI_SOURCE);
                manager.incrementackM();
            }
            else if(status.MPI_TAG == REQ_FOR_K)
            {
                printf("%d dostalem REQ K od %d\n", myProcRank, status.MPI_SOURCE);
                manager.addProcesToDockQueue({status.MPI_SOURCE, come_lamp_value});
                manager.printPrcessInDockQueue();
            }
            else if(status.MPI_TAG == REQ_FOR_M)
            {
                printf("%d dostalem REQ M od %d\n", myProcRank, status.MPI_SOURCE);
                manager.addProcesToMechanicQueue({status.MPI_SOURCE, come_lamp_value});
                manager.printProcessInMechanicQueue();
            }

        }
    }
}