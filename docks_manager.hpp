#ifndef DOCK_MANAGER_HPP
#define DOCK_MANAGER_HPP

#include "queue_manager.hpp"

class DockManager : public QueueManager
{
public:
    DockManager(int _size, int _rank) : QueueManager(_size, _rank) {}
    
    bool startDocking();
    bool endDocking();
private:
};

#endif //! DOCK_MANAGER_HPP