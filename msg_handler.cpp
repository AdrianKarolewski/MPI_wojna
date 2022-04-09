#include "msg_handler.hpp"

#include <mpi.h>

void msg_handler(const int &_rank, const int &_size, DockManager &dockManager, MechanicsManager &mechanicsManager)
{
    // printf("Watek komunikacyjny START %d!\n", _rank);
    const int myProcRank{_rank};
    const int procSize{_size};

    int come_lamp_value;
    MPI_Status status;

    while (true)
    {
        MPI_Recv(&come_lamp_value, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_SOURCE == myProcRank)
        {
            if (status.MPI_TAG == REQ_FOR_K)
            {
                printf("%d ubiega sie o dostep do dokow\n", myProcRank);
                // aktualicujemy zegar zgodny z aktualnym stanem managera mogl w miedzy czasie wyslac ack np lub odebrac req
                dockManager.clock_increment();
                come_lamp_value = dockManager.get_lamport_clock();

                // dodajemy sie do kolejki
                dockManager.addProcesTokQueue({status.MPI_SOURCE, come_lamp_value});
                dockManager.printPrcessInQueue();
                // rozsylamy do wszystkich innych procesow REQ_K
                for (int i = 0; i < procSize; ++i)
                {
                    if (i != myProcRank)
                    {
                        MPI_Send(&come_lamp_value, 1, MPI_INT, i, REQ_FOR_K, MPI_COMM_WORLD);
                    }
                }
            }
            else if (status.MPI_TAG == REQ_FOR_M)
            {
                printf("%d ubiega sie o dostep do mechanikow\n", myProcRank);
                // aktualicujemy zegar zgodny z aktualnym stanem managera mogl w miedzy czasie wyslac ack np lub odebrac req
                mechanicsManager.clock_increment();
                come_lamp_value = mechanicsManager.get_lamport_clock();
                // dodajemy sie do kolejki
                mechanicsManager.addProcesTokQueue({status.MPI_SOURCE, come_lamp_value});
                mechanicsManager.printPrcessInQueue();
                // rozsylamy do wszystkich innych procesow REQ_M
                for (int i = 0; i < procSize; ++i)
                {
                    if (i != myProcRank)
                    {
                        MPI_Send(&come_lamp_value, 1, MPI_INT, i, REQ_FOR_M, MPI_COMM_WORLD);
                    }
                }
            }
            // gdy nasz proces wyjdzie z sekcji krytycznej K
            else if (status.MPI_TAG == ACK_FOR_K)
            {
                // pobieramy wszystkie procesy w kolejce za nami
                auto ptoACK = dockManager.afterMeInQueue();
                // inkrementujemy zegar
                dockManager.clock_increment();
                int c = dockManager.get_lamport_clock();
                for (auto p : ptoACK)
                {
                    MPI_Send(&c, 1, MPI_INT, p, ACK_FOR_K, MPI_COMM_WORLD);
                }
                // usuwamy sie z kolejki
                dockManager.delProcesFromQueue(myProcRank);
                dockManager.printPrcessInQueue();
            }
            // gdy nasz proces wyjdzie z sekcji krytycznej M
            else if (status.MPI_TAG == ACK_FOR_M)
            {
                // pobieramy wszystkie procesy w kolejce za nami
                auto ptoACK = mechanicsManager.afterMeInQueue();
                mechanicsManager.clock_increment();
                int c = mechanicsManager.get_lamport_clock();
                for (auto p : ptoACK)
                {
                    printf("poMnie %d", 1);
                    MPI_Send(&c, 1, MPI_INT, p, ACK_FOR_M, MPI_COMM_WORLD);
                }
                // usuwamy sie z kolejki
                mechanicsManager.delProcesFromQueue(myProcRank);
                mechanicsManager.printPrcessInQueue();
            }
        }
        // wiadomosci od innych procesow
        else
        {
            if (status.MPI_TAG == ACK_FOR_K)
            {
                dockManager.clock_increment(come_lamp_value);
                if (dockManager.myClockInQueue() <= come_lamp_value) // zapewnia nam ze nie odbierzemy starych ack dla nowego zapytania
                {
                    // printf("%d dostalem ACK K od %d\n", myProcRank, status.MPI_SOURCE);
                    //  jezeli dal mi ack a mial lepsza pozycje znaczy ze wyszedl z krytycznej
                    if (dockManager.hasBetterPositionInQueue(status.MPI_SOURCE))
                    {
                        dockManager.delProcesFromQueue(status.MPI_SOURCE);
                    }
                    dockManager.incrementACK();
                }
                dockManager.printPrcessInQueue();
            }
            else if (status.MPI_TAG == ACK_FOR_M)
            {
                mechanicsManager.clock_increment(come_lamp_value);
                if (mechanicsManager.myClockInQueue() <= come_lamp_value) // zapewnie nam ze nie odbierzemy starych ack dla nowego zapytania
                {
                    // printf("%d dostalem ACK M od %d\n", myProcRank, status.MPI_SOURCE);

                    // jezeli dal mi ack a mial lepsza pozycje znaczy ze wyszedl z krytycznej
                    if (mechanicsManager.hasBetterPositionInQueue(status.MPI_SOURCE))
                    {
                        mechanicsManager.delProcesFromQueue(status.MPI_SOURCE);
                    }
                    mechanicsManager.incrementACK();
                }
                mechanicsManager.printPrcessInQueue();
            }
            else if (status.MPI_TAG == REQ_FOR_K)
            {
                // printf("%d dostalem REQ K od %d\n", myProcRank, status.MPI_SOURCE);
                dockManager.clock_increment(come_lamp_value);
                dockManager.delProcesFromQueue(status.MPI_SOURCE);
                dockManager.addProcesTokQueue({status.MPI_SOURCE, come_lamp_value});
                dockManager.printPrcessInQueue();

                if (dockManager.hasBetterPositionInQueue(status.MPI_SOURCE))
                {
                    dockManager.clock_increment();
                    int c = dockManager.get_lamport_clock();

                    MPI_Send(&c, 1, MPI_INT, status.MPI_SOURCE, ACK_FOR_K, MPI_COMM_WORLD);
                }
            }
            else if (status.MPI_TAG == REQ_FOR_M)
            {
                // printf("%d dostalem REQ M od %d\n", myProcRank, status.MPI_SOURCE);
                mechanicsManager.clock_increment(come_lamp_value);
                mechanicsManager.delProcesFromQueue(status.MPI_SOURCE);
                mechanicsManager.addProcesTokQueue({status.MPI_SOURCE, come_lamp_value});
                mechanicsManager.printPrcessInQueue();
                if (mechanicsManager.hasBetterPositionInQueue(status.MPI_SOURCE))
                {
                    mechanicsManager.clock_increment();
                    int c = mechanicsManager.get_lamport_clock();
                    MPI_Send(&c, 1, MPI_INT, status.MPI_SOURCE, ACK_FOR_M, MPI_COMM_WORLD);
                }
            }
        }
    }
}