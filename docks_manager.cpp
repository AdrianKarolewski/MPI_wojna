#include "docks_manager.hpp"

#include <mpi.h>

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>

// funkcja obslugujaca ubieganie sie o doki dla naszego procesu zakolejkowanie do sekcji krytycznej dokow
void DockManager::printPrcessInQueue()
{
    //printf("%d moja kolejka do dokow: ", myRank);
    //QueueManager::printPrcessInQueue();
}
bool DockManager::startDocking()
{
    int tab[2];
    // wysylamy req do siebie ze chcemy dostep do M
    MPI_Send(&tab, 2, MPI_INT, myRank, REQ_FOR_K, MPI_COMM_WORLD);

    // czekamy za odpowiednia iloscia ack
    while (ack < nprocesInWork - K_DOCKS)
    {
        sleep(0.1);
    }
    // wchodzimy do sekcji krytycznej
    printf("%d zajmuje dok DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n", myRank);
    return true;
}
// funkcja konczaca dokowanie naszego procesu wykolejkowanie z sekcji krytycznej dokow
bool DockManager::endDocking()
{
    int tab[2];
    ack = 0;
    // gdy wszystko od dajemy znak o opuszenie sekcji krytycznej
    printf("%d opuszcza dok DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n", myRank);
    MPI_Send(&tab, 2, MPI_INT, myRank, ACK_FOR_K, MPI_COMM_WORLD);
    return true;
}