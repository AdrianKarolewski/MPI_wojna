#ifndef QUEUE_MANAGER_HPP
#define QUEUE_MANAGER_HPP

#include <atomic>
#include <mutex>
#include <set>
#include <vector>

#define M_MECHANICS 15
#define K_DOCKS 3

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

class QueueManager
{
public:
    QueueManager(int _size, int _rank) : lamport_clock(1), ack(0), myRank(_rank), nprocesInWork(_size) {}

    void clock_increment(int c_value_come = 0);
    int get_lamport_clock();
    int myClockInQueue();

    void addProcesTokQueue(ProcessInQueue p);
    void delProcesFromQueue(int pId);
    void printPrcessInQueue();

    bool hasBetterPositionInQueue(int pId);
    std::vector<int> afterMeInQueue();

    void incrementACK() { ack++; }

protected:
    const int myRank;
    const int nprocesInWork;
    std::atomic_int ack;

private:
    std::atomic_int lamport_clock;
    
    std::set<ProcessInQueue, ProcessInQueueCmp> processWaitingInQueue;
    std::mutex setMutex;
};

#endif //! QUEUE_MANAGER_HPP