#include "mechanics_manager.hpp"

#include <mpi.h>

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>

int global_mechanic_number = M_MECHANICS; // ilosc mechanikow do modyfikowania zmienna globalna dla wszyskich procesow

void MechanicsManager::printPrcessInQueue()
{
    printf("%d moja kolejka do mechanikow: ", myRank);
    QueueManager::printPrcessInQueue();
    printf("\n");
}

bool MechanicsManager::takeAccesToGlobalVMech(int m_number)
{
    int lamp_nr = get_lamport_clock();
    // wysylamy req do siebie ze chcemy dostep do M
    MPI_Send(&lamp_nr, 1, MPI_INT, myRank, REQ_FOR_M, MPI_COMM_WORLD);

    // czekamy za odpowiednia iloscia ack
    while (ack < K_DOCKS - 1)
    {
        sleep(0.2);
    }

    // wchodzimy do sekcji krytycznej
    printf("%d wchodzi do sekcji strzezonej mechanikow ich ilosc - %d\n", myRank, global_mechanic_number);
    if (global_mechanic_number - m_number >= 0)
    {
        global_mechanic_number -= m_number;
    }
    else // gdy za malo mechanikow zwalniamy sekcje krytyczna i idziemy na koniec kolejki
    {
        printf("%d opuszcza sekcje krytyczna mechanikow - brak wymaganej ilosci mechanikow\n", myRank);
        ack = 0;
        int lamp_nr = get_lamport_clock();
        // znak o opuszczenie sekcji krytycznej
        MPI_Send(&lamp_nr, 1, MPI_INT, myRank, ACK_FOR_M, MPI_COMM_WORLD);
        // znak o checi zakolejkowania
        MPI_Send(&lamp_nr, 1, MPI_INT, myRank, REQ_FOR_M, MPI_COMM_WORLD);
        return false;
    }
    // gdy wszystko od dajemy znak o opuszenie sekcji krytycznej
    printf("%d opuszcza sekcje krytyczna mechanikow\n", myRank);
    ack = 0;
    MPI_Send(&lamp_nr, 1, MPI_INT, myRank, ACK_FOR_M, MPI_COMM_WORLD);
    return true;
}
// pobiera z zmiennej globalnej podana ilosc mechanikow
bool MechanicsManager::takeMechanick(int m_number)
{
    printf("Chce pobrac mechanikow %d - proc: %d\n",m_number ,myRank);
    if(m_number > 0)
    {
        return takeAccesToGlobalVMech(m_number);
    }
    return true;
}

// oddaje do zmiennej globalnej podana ilosc mechanikow
bool MechanicsManager::releaseMechanic(int m_number)
{
    printf("Chce oddac mechanikow %d -proc: %d\n",m_number, myRank);
    if(m_number > 0)
    {
        return takeAccesToGlobalVMech(-(m_number));
    }
    return true;
}