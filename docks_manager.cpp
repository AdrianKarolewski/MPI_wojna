#include "docks_manager.hpp"

#include <mpi.h>

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>

int global_mechanic_number = M_MECHANICS; // ilosc mechanikow do modyfikowania zmienna globalna dla wszyskich procesow

// inkremenatcja zegaru lamporta, przypisuje max() + 1 z wartosci zegaru obecnej lub wartosci ktora przyszla w wiadomosci
void DockManager::clock_increment(int c_value_come)
{
    if (lamport_clock > c_value_come)
    {
        lamport_clock++;
        return;
    }
    lamport_clock = c_value_come + 1;
}
// pobranie wartosci zegara lamporta dla procesu
int DockManager::get_lamport_clock()
{
    return lamport_clock;
}
// dodanie procesu ubiegajacego sie o dostep do dokow
void DockManager::addProcesToDockQueue(ProcessInQueue p)
{
    ForDockSetMutex.lock();
    processWaitingForDock.insert(ProcessInQueue(p));
    ForDockSetMutex.unlock();
}

// dodanie procesu ubiegajacego sie o dostep do mechanikow
void DockManager::addProcesToMechanicQueue(ProcessInQueue p)
{
    ForDockSetMutex.lock();
    processWaitingForMechanic.insert(ProcessInQueue(p));
    ForDockSetMutex.unlock();
}
void DockManager::delProcesFromDockQueue(ProcessInQueue p)
{
    ForDockSetMutex.lock();
    // TODO
    ForDockSetMutex.unlock();
}
void DockManager::delProcesFromMechanicQueue(ProcessInQueue p)
{
    ForDockSetMutex.lock();
    // TODO
    ForDockSetMutex.unlock();
}
void DockManager::printPrcessInDockQueue()
{
    ForDockSetMutex.lock();
    printf("%d moja kolejka do Dokow: ", myRank);
    for (auto p : processWaitingForMechanic)
    {
        printf("p: %d l: %d -> ", p.processRank, p.lamportClockValue);
    }
    printf("\n");
    ForDockSetMutex.unlock();
}
void DockManager::printProcessInMechanicQueue()
{
    ForDockSetMutex.lock();
    printf("%d moja kolejka do Mechanikow: ", myRank);
    for (auto p : processWaitingForMechanic)
    {
        printf("p: %d l: %d ->", p.processRank, p.lamportClockValue);
    }
    printf("\n");
    ForDockSetMutex.unlock();
}
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
    ackK = 0;
    return true;
}
// pobieranie mechanikow dla naszego procesu wejscie do sekcji krytycznej mechanikow i ich pobranie
bool DockManager::takeMechanick(int m_number)
{
    mechanic_number = m_number;
    int lamp_nr = lamport_clock;
    MPI_Send(&lamp_nr, 1, MPI_INT, myRank, REQ_FOR_M, MPI_COMM_WORLD);

    while (ackM < K_DOCKS - 1)
    {
        // printf("%d czekam na ack", myRank);
        sleep(0.5);
    }
    printf("%d wchodzi do sekcji strzezonej mechanikow %d\n", myRank, global_mechanic_number);
    if (global_mechanic_number - mechanic_number >= 0)
    {
        global_mechanic_number -= mechanic_number;
    }
    else
    {
        printf("%d opuszcza sekcje krytyczna mechanikow - brak wymaganej ilosci mechanikow\n", myRank);
        ackM = 0;
        return false;
    }
    printf("%d opuszcza sekcje krytyczna mechanikow - rozpoczynam naprawy\n", myRank);

    return true;
}
// zwracanie mechanikow wejscie do sekcji krytycznej mechanikow i ich oddanie
bool DockManager::releaseMechanic()
{
    // MPI_Send(&lamport_clock , 1 , MPI_INT , myRank , REQ_FOR_M , MPI_COMM_WORLD);
    while (ackM < K_DOCKS - 1)
    {
        sleep(0.5);
    }
    printf("%d wchodzi do sekcji strzezonej mechanikow\n", myRank);
    global_mechanic_number += mechanic_number;
    ackM = 0;
    return true;
}