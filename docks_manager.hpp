#ifndef DOCK_MANAGER_HPP
#define DOCK_MANAGER_HPP

#include "queue_manager.hpp"

class DockManager : public QueueManager
{
public:
    DockManager(int _size, int _rank) : QueueManager(_size, _rank), isInDock(false) {}
    bool startDocking();
    bool endDocking();
    void printPrcessInQueue();

    std::atomic_bool isInDock;
};
#endif //! DOCK_MANAGER_HPP