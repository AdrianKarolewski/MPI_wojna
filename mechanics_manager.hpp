#ifndef MECHANICS_MANAGER_HPP
#define MECHANICS_MANAGER_HPP

#include "queue_manager.hpp"

class MechanicsManager : public QueueManager
{
public:
    MechanicsManager(int _size, int _rank) : QueueManager(_size, _rank) {}
    
    bool takeMechanick(int m_number);
    bool releaseMechanic(int m_number);
    bool takeAccesToGlobalVMech(int m_number);
    void printPrcessInQueue();
};

#endif //! MECHANICS_MANAGER_HPP