#include "mechanics_manager.hpp"

#include <mpi.h>

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>

void MechanicsManager::printPrcessInQueue()
{
    printf("%d moja kolejka do mechanikow: ", myRank);
    QueueManager::printPrcessInQueue();
    printf("\n");
}

bool MechanicsManager::takeAccesToGlobalVMech(int m_number)
{
    int tab[2];
    tab[0] = get_lamport_clock();
    tab[1] = global_mechanic_number;
    // wysylamy req do siebie ze chcemy dostep do M
    MPI_Send(&tab, 2, MPI_INT, myRank, REQ_FOR_M, MPI_COMM_WORLD);

    // czekamy za odpowiednia iloscia ack
    while (ack < nprocesInWork - 1)
    {
        sleep(0.1);
    }
    // wchodzimy do sekcji krytycznej
    printf("%d wchodzi do sekcji strzezonej mechanikow ich ilosc - %d\n", myRank, global_mechanic_number);
    if (global_mechanic_number - m_number >= 0)
    {
        global_mechanic_number -= m_number;
    }
    else // gdy za malo mechanikow zwalniamy sekcje krytyczna i idziemy na koniec kolejki
    {
        printf("%d opuszcza sekcje krytyczna mechanikow - brak wymaganej ilosci mechanikow - %d\n", myRank, global_mechanic_number);
        ack = 0;
        // znak o opuszczenie sekcji krytycznej
        MPI_Send(&tab, 2, MPI_INT, myRank, ACK_FOR_M, MPI_COMM_WORLD);
        // znak o checi zakolejkowania
        return false;
    }
    // gdy wszystko od dajemy znak o opuszenie sekcji krytycznej
    tab[1] = global_mechanic_number;
    printf("%d opuszcza sekcje krytyczna mechanikow ich ilsc - %d \n", myRank, global_mechanic_number);
    ack = 0;
    MPI_Send(&tab, 2, MPI_INT, myRank, ACK_FOR_M, MPI_COMM_WORLD);
    return true;
}
// pobiera z zmiennej globalnej podana ilosc mechanikow
bool MechanicsManager::takeMechanick(int m_number)
{
    printf("Chce pobrac mechanikow %d - proc: %d\n",m_number ,myRank);
    if(m_number > 0)
    {
        //return takeAccesToGlobalVMech(m_number);
    }
    return true;
}

// oddaje do zmiennej globalnej podana ilosc mechanikow
bool MechanicsManager::releaseMechanic(int m_number)
{
    printf("Chce oddac mechanikow %d -proc: %d\n",m_number, myRank);
    if(m_number > 0)
    {
        //return takeAccesToGlobalVMech(-(m_number));
    }
    return true;
}