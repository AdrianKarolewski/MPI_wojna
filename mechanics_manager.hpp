#ifndef MECHANICS_MANAGER_HPP
#define MECHANICS_MANAGER_HPP

#include "queue_manager.hpp"

class MechanicsManager : public QueueManager
{
public:
    MechanicsManager(int _size, int _rank) : QueueManager(_size, _rank), global_mechanic_number(M_MECHANICS) {}
    
    bool takeMechanick(int m_number);
    bool releaseMechanic(int m_number);
    bool takeAccesToGlobalVMech(int m_number);
    void printPrcessInQueue();
    void setGlobalMechanicNumber(int x) { global_mechanic_number = x; }
    int getGlobalMechanicNumber() { return global_mechanic_number; }
private:
    int global_mechanic_number; // ilosc mechanikow do modyfikowania zmienna globalna dla wszyskich procesow
};

#endif //! MECHANICS_MANAGER_HPP