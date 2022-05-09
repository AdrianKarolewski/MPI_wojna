#ifndef MECHANICS_MANAGER_HPP
#define MECHANICS_MANAGER_HPP

#include "queue_manager.hpp"

class MechanicsManager : public QueueManager
{
public:
    MechanicsManager(int _size, int _rank) : QueueManager(_size, _rank), mWeNeed(0) {}
    
    bool takeMechanick(int m_number);
    bool releaseMechanic(int m_number);
    void printPrcessInQueue();
    int how_much_we_need() const { return mWeNeed; }
private:
    std::atomic_int mWeNeed;
};

#endif //! MECHANICS_MANAGER_HPP