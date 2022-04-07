#include "docks_manager.hpp"

#include <mpi.h>

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>

// funkcja obslugujaca ubieganie sie o doki dla naszego procesu zakolejkowanie do sekcji krytycznej dokow
bool DockManager::startDocking()
{
    // TODO
    /*
    while(ackK < nprocesInWork - dock_number)
    {

    }
    */
    // printf("%d zadokowalem\n", myRank);
    return true;
}
// funkcja konczaca dokowanie naszego procesu wykolejkowanie z sekcji krytycznej dokow
bool DockManager::endDocking()
{
    ack = 0;
    return true;
}