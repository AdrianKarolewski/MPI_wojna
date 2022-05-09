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
// zajmuje okreslony rozmiar sekcji krytycznej mechanikow
bool MechanicsManager::takeMechanick(int m_number)
{
    printf("Chce zajaz mechanikow %d - proc: %d\n",m_number ,myRank);
    int tab[2];
    mWeNeed = m_number;
    // wysylamy req do siebie ze chcemy dostep do M
    MPI_Send(&tab, 2, MPI_INT, myRank, REQ_FOR_M, MPI_COMM_WORLD);

    // czekamy za odpowiednia iloscia ack
    while (ack < M_MECHANICS * (nprocesInWork - 2) + m_number)
    {
        sleep(0.1);
    }

    // wchodzimy do sekcji krytycznej
    printf("%d wchodzi do sekcji strzezonej mechanikow zajol - %d\n", myRank, m_number);
    
    return true;
}
// zwalnia okreslony rozmiar sekcji krytycznej mechanikow
bool MechanicsManager::releaseMechanic(int m_number)
{
    int tab[2];
    printf("Chce zwolnic mechanikow %d - proc: %d\n",m_number, myRank);

    tab[MECHANIC_V_IN_TAB] = m_number;
    // gdy wszystko od dajemy znak o opuszenie sekcji krytycznej
    printf("%d opuszcza sekcje krytyczna mechanikow zwolnil - %d \n", myRank, m_number);
    MPI_Send(&tab, 2, MPI_INT, myRank, ACK_FOR_M, MPI_COMM_WORLD);
    
    ack = 0;
    mWeNeed = 0;
    return true;
    
}