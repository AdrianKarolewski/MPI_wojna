#include "mechanics_manager.hpp"

#include <mpi.h>

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>

int global_mechanic_number = M_MECHANICS; // ilosc mechanikow do modyfikowania zmienna globalna dla wszyskich procesow

bool MechanicsManager::takeMechanick(int m_number)
{
    mechanic_number = m_number;
    int lamp_nr = get_lamport_clock();
    MPI_Send(&lamp_nr, 1, MPI_INT, myRank, REQ_FOR_M, MPI_COMM_WORLD);

    while (ack < K_DOCKS - 1)
    {
        sleep(0.2);
    }
    printf("%d wchodzi do sekcji strzezonej mechanikow %d\n", myRank, global_mechanic_number);
    if (global_mechanic_number - mechanic_number >= 0)
    {
        global_mechanic_number -= mechanic_number;
    }
    else
    {
        printf("%d opuszcza sekcje krytyczna mechanikow - brak wymaganej ilosci mechanikow\n", myRank);
        ack = 0;
        int lamp_nr = get_lamport_clock();
        MPI_Send(&lamp_nr, 1, MPI_INT, myRank, REQ_FOR_M, MPI_COMM_WORLD);
        return false;
    }
    printf("%d opuszcza sekcje krytyczna mechanikow - rozpoczynam naprawy\n", myRank);
    ack = 0;
    MPI_Send(&lamp_nr, 1, MPI_INT, myRank, ACK_FOR_M, MPI_COMM_WORLD);
    delProcesFromQueue(myRank);
    return true;
}
// zwracanie mechanikow wejscie do sekcji krytycznej mechanikow i ich oddanie
bool MechanicsManager::releaseMechanic()
{
    // MPI_Send(&lamport_clock , 1 , MPI_INT , myRank , REQ_FOR_M , MPI_COMM_WORLD);
    /*
    while (ack < K_DOCKS - 1)
    {
        sleep(0.5);
    }
    printf("%d wchodzi do sekcji strzezonej mechanikow\n", myRank);
    global_mechanic_number += mechanic_number;
    ack = 0;
    */
    return true;
}