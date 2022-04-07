#include "msg_handler.hpp"

#include <mpi.h>


void msg_handler(const int & _rank, const int & _size, DockManager & dockManager, MechanicsManager & mechanicsManager)
{
    //printf("Watek komunikacyjny START %d!\n", _rank);
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
                come_lamp_value = dockManager.get_lamport_clock(); // aktualicujemy zegar zgodny z aktualnym stanem managera mogl w miedzy czasie wyslac ack np lub odebrac req
                dockManager.addProcesTokQueue({status.MPI_SOURCE, come_lamp_value});
                dockManager.printPrcessInQueue();
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
                come_lamp_value = mechanicsManager.get_lamport_clock(); // aktualicujemy zegar zgodny z aktualnym stanem managera mogl w miedzy czasie wyslac ack np lub odebrac req
                mechanicsManager.addProcesTokQueue({status.MPI_SOURCE, come_lamp_value});
                mechanicsManager.printPrcessInQueue();
                for(int i = 0; i < procSize; ++i)
                {
                    if(i != myProcRank)
                    {
                        MPI_Send(&come_lamp_value , 1, MPI_INT , i , REQ_FOR_M , MPI_COMM_WORLD);
                    }
                }
            }
            else if(status.MPI_TAG == ACK_FOR_K)
            {
                auto ptoACK = dockManager.afterMeInQueue();
                for(auto p : ptoACK)
                {
                    dockManager.clock_increment();
                    int c = dockManager.get_lamport_clock();
                    printf("ack z %d\n", c);
                    MPI_Send(&c, 1 , MPI_INT , p, ACK_FOR_K, MPI_COMM_WORLD);
                }
            }
            else if(status.MPI_TAG == ACK_FOR_M)
            {
                auto ptoACK = mechanicsManager.afterMeInQueue();
                for(auto p : ptoACK)
                {
                    mechanicsManager.clock_increment();
                    int c = mechanicsManager.get_lamport_clock();
                    printf("ack z %d\n", c);
                    MPI_Send(&c, 1 , MPI_INT , p, ACK_FOR_M, MPI_COMM_WORLD);
                }
                 
            }
        }
        else
        {
            if(status.MPI_TAG == ACK_FOR_K)
            {         
                if(dockManager.myClockInQueue() <= come_lamp_value) // zapewnia nam ze nie odbierzemy starych ack dla nowego zapytania
                {
                    printf("%d dostalem ACK K od %d\n", myProcRank, status.MPI_SOURCE);
                    dockManager.incrementACK();
                    // jezeli dal mi ack a mial lepsza pozycje znaczy ze wyszedl z krytycznej
                    if(dockManager.hasBetterPositionInQueue(status.MPI_SOURCE))
                    {
                        dockManager.delProcesFromQueue(status.MPI_SOURCE);
                    }
                }
                
            }
            else if(status.MPI_TAG == ACK_FOR_M)
            {
                if(mechanicsManager.myClockInQueue() <= come_lamp_value) // zapewnie nam ze nie odbierzemy starych ack dla nowego zapytania
                {
                    printf("%d dostalem ACK M od %d\n", myProcRank, status.MPI_SOURCE);
                    mechanicsManager.incrementACK();
                    // jezeli dal mi ack a mial lepsza pozycje znaczy ze wyszedl z krytycznej
                    if(mechanicsManager.hasBetterPositionInQueue(status.MPI_SOURCE))
                    {
                        mechanicsManager.delProcesFromQueue(status.MPI_SOURCE);
                    }
                }
                
            }
            else if(status.MPI_TAG == REQ_FOR_K)
            {
                //printf("%d dostalem REQ K od %d\n", myProcRank, status.MPI_SOURCE);
                dockManager.clock_increment(come_lamp_value);
                dockManager.addProcesTokQueue({status.MPI_SOURCE, come_lamp_value});
                dockManager.printPrcessInQueue();
                if(dockManager.hasBetterPositionInQueue(status.MPI_SOURCE))
                {
                    dockManager.clock_increment();
                    int c = dockManager.get_lamport_clock();
                    
                    MPI_Send(&c, 1 , MPI_INT , status.MPI_SOURCE, ACK_FOR_K, MPI_COMM_WORLD);
                    
                }
            }
            else if(status.MPI_TAG == REQ_FOR_M)
            {
                //printf("%d dostalem REQ M od %d\n", myProcRank, status.MPI_SOURCE);
                mechanicsManager.clock_increment(come_lamp_value);
                mechanicsManager.addProcesTokQueue({status.MPI_SOURCE, come_lamp_value});
                mechanicsManager.printPrcessInQueue();
                if(mechanicsManager.hasBetterPositionInQueue(status.MPI_SOURCE))
                {
                    mechanicsManager.clock_increment();
                    int c = mechanicsManager.get_lamport_clock();
                    printf("ack z %d\n", c);
                    MPI_Send(&c, 1 , MPI_INT , status.MPI_SOURCE, ACK_FOR_M, MPI_COMM_WORLD);
                }
            }

        }
    }
}