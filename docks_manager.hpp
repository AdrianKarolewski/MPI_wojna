#ifndef DOCK_MANAGER_HPP
#define DOCK_MANAGER_HPP

#include <atomic>
#include <mutex>
#include <set>

struct ProcessInQueue
{
    int processRank;
    int lamportClockValue;
    ProcessInQueue(int pR, int lCV) : processRank(pR), lamportClockValue(lCV) {}
};
struct ProcessInQueueCmp
{
    bool operator()(const ProcessInQueue & p1, const ProcessInQueue & p2)
    {
        return p1.lamportClockValue < p2.lamportClockValue;
    }
};

class DockManager
{
public:
    DockManager() : lamport_clock(1), ackK(0), ackM(0) {}
    void clock_increment(int c_value_come = 0);
    int get_lamport_clock();
    void addProcesToDockQueue(ProcessInQueue p);
    void addProcesToMechanicQueue(ProcessInQueue p);
    bool startDocking();
    bool endDocking();
    bool takeMechanick();
    bool releaseMechanic();
    void incrementackK() {ackK++;}
    void incrementackM() {ackM++;}
private:
    std::atomic_int lamport_clock;
    std::atomic_int ackK;
    std::atomic_int ackM;
    std::set<ProcessInQueue> processWaitingForDock;
    std::set<ProcessInQueue> processWaitingForMechanic;
    std::mutex ForDockSetMutex;
    std::mutex ForMechanicSetMutex;
};

#endif //! DOCK_MANAGER_HPP