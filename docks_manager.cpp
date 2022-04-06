#include "docks_manager.hpp"

void DockManager::clock_increment(int c_value_come = 0)
{
    if (lamport_clock > c_value_come)
    {
        lamport_clock++;
        return;
    }
    lamport_clock = c_value_come + 1;
}
int DockManager::get_lamport_clock()
{
    return lamport_clock;
}
void DockManager::addProcesToDockQueue(ProcessInQueue p)
{
    ForDockSetMutex.lock();
    processWaitingForDock.insert(ProcessInQueue(p));
    ForDockSetMutex.unlock();
}
void DockManager::addProcesToMechanicQueue(ProcessInQueue p)
{
    ForDockSetMutex.lock();
    processWaitingForMechanic.insert(ProcessInQueue(p));
    ForDockSetMutex.unlock();
}
bool DockManager::startDocking()
{
    //TODO
    while(ackK < 10)
    {

    }
}
// zerowanie potwierdzen dla dokowania
bool DockManager::endDocking()
{
    
    ackK = 0;
    
}
bool DockManager::takeMechanick()
{
    //TODO
    while(ackM < 10)
    {
        
    }

}
// zerowanie potwierdzen dla mechanikow
bool DockManager::releaseMechanic()
{
   
    ackM = 0;

}