#include "queue_manager.hpp"

#include <mpi.h>

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>

// inkremenatcja zegaru lamporta, przypisuje max() + 1 z wartosci zegaru obecnej lub wartosci ktora przyszla w wiadomosci
void QueueManager::clock_increment(int c_value_come)
{
    if (lamport_clock > c_value_come)
    {
        lamport_clock++;
        return;
    }
    lamport_clock = c_value_come + 1;
}
// pobranie wartosci zegara lamporta dla procesu
int QueueManager::get_lamport_clock()
{
    return lamport_clock;
}
int QueueManager::myClockInQueue()
{
    for (const auto &p : processWaitingInQueue)
    {
        if (p.processRank == myRank)
        {
            return p.lamportClockValue;
        }
    }
    return 9999;
}
// dodanie procesu ubiegajacego sie o dostep do zmiennych chronionych
void QueueManager::addProcesTokQueue(ProcessInQueue p)
{
    setMutex.lock();
    processWaitingInQueue.insert(ProcessInQueue(p));
    setMutex.unlock();
}

void QueueManager::delProcesFromQueue(int pId)
{
    setMutex.lock();
    for(auto it = processWaitingInQueue.begin(); it != processWaitingInQueue.end(); ++it)
    {
        if((*it).processRank == pId)
        {
            processWaitingInQueue.erase((*it));
            break;
        }
    }
    setMutex.unlock();
}
bool QueueManager::hasBetterPositionInQueue(int pId)
{
    int mpos = processWaitingInQueue.size() + 1; // w wypadku nie znalezienia w kolejce siebie wyslemy cawsze ack
    int cpos = processWaitingInQueue.size() + 2;
    int i{0};
    for (const auto &p : processWaitingInQueue)
    {
        if (p.processRank == myRank)
        {
            mpos = i;
        }
        else if (p.processRank == pId)
        {
            cpos = i;
        }
        ++i;
    }
    return cpos < mpos;
}
std::vector<int> QueueManager::afterMeInQueue()
{
    int mpos = 0; // w wypadku nie znalezienia w kolejce siebie wyslemy cawsze ack
    int i{0};
    std::vector<int> afterme;
    for (const auto &p : processWaitingInQueue)
    {
        if (p.processRank == myRank)
        {
            mpos = i;
        }
        i++;
    }
    auto it = processWaitingInQueue.begin();
    for(int i = 0; i <= mpos; ++i)
    {
        ++it;
    }
    for(; it != processWaitingInQueue.end(); ++it)
    {
        afterme.emplace_back((*it).processRank);
    }
    return afterme;
}
void QueueManager::printPrcessInQueue()
{
    setMutex.lock();
    printf(" moj aktualny lamport %d  |", get_lamport_clock());
    for (const auto &p : processWaitingInQueue)
    {
        printf("%d l:%d -> ", p.processRank, p.lamportClockValue);
    }
    setMutex.unlock();
    int a = ack;
    printf("ilosc moich ack %d \n",a);
}