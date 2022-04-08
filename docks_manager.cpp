#include "docks_manager.hpp"

#include <mpi.h>

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>

// funkcja obslugujaca ubieganie sie o doki dla naszego procesu zakolejkowanie do sekcji krytycznej dokow
void DockManager::printPrcessInQueue()
{
    printf("%d moja kolejka do dokow: ", myRank);
    QueueManager::printPrcessInQueue();
}
bool DockManager::startDocking()
{
    return true;
}
// funkcja konczaca dokowanie naszego procesu wykolejkowanie z sekcji krytycznej dokow
bool DockManager::endDocking()
{
    return true;
}