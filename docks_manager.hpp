#ifndef DOCK_MANAGER_HPP
#define DOCK_MANAGER_HPP

#include <atomic>
#include <mutex>
#include <set>

#define M_MECHANICS 100
#define K_DOCKS 5

#define ACK_FOR_K 100
#define REQ_FOR_K 101

#define ACK_FOR_M 102
#define REQ_FOR_M 103

// wspolne zmienne globalne dla procesow

struct ProcessInQueue
{
    int processRank;
    int lamportClockValue;
    ProcessInQueue(int pR, int lCV) : processRank(pR), lamportClockValue(lCV) {}
};
struct ProcessInQueueCmp
{
    bool operator()(const ProcessInQueue &p1, const ProcessInQueue &p2)
    {
        if (p1.lamportClockValue != p2.lamportClockValue)
        {
            return p1.lamportClockValue < p2.lamportClockValue;
        }
        return p1.processRank < p2.processRank;
    }
};

class DockManager
{
public:
    DockManager(int _size, int _rank) : lamport_clock(1), ackK(0), ackM(0), myRank(_rank), nprocesInWork(_size), mechanic_number(0) {}
    void clock_increment(int c_value_come = 0);
    int get_lamport_clock();
    void addProcesToDockQueue(ProcessInQueue p);
    void addProcesToMechanicQueue(ProcessInQueue p);

    void delProcesFromDockQueue(ProcessInQueue p);
    void delProcesFromMechanicQueue(ProcessInQueue p);

    void printPrcessInDockQueue();
    void printProcessInMechanicQueue();
    
    bool startDocking();
    bool endDocking();
    bool takeMechanick(int m_number);
    bool releaseMechanic();
    void incrementackK() { ackK++; }
    void incrementackM() { ackM++; }


private:
    std::atomic_int lamport_clock;
    std::atomic_int ackK;
    std::atomic_int ackM;
    const int myRank;
    const int nprocesInWork;

    int mechanic_number;

    std::set<ProcessInQueue, ProcessInQueueCmp> processWaitingForDock;
    std::set<ProcessInQueue, ProcessInQueueCmp> processWaitingForMechanic;
    std::mutex ForDockSetMutex;
    std::mutex ForMechanicSetMutex;
};

#endif //! DOCK_MANAGER_HPP