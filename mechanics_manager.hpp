#ifndef MECHANICS_MANAGER_HPP
#define MECHANICS_MANAGER_HPP

#include "queue_manager.hpp"

class MechanicsManager : public QueueManager
{
public:
    MechanicsManager(int _size, int _rank) : QueueManager(_size, _rank), mechanic_number(0) {}
    
    bool startDocking();
    bool endDocking();
    bool takeMechanick(int m_number);
    bool releaseMechanic();
private:
    int mechanic_number;
};

#endif //! MECHANICS_MANAGER_HPP