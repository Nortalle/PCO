#include <QMutex>
#include <QWaitCondition>

#define CARWEIGHT 1
#define TRUCKWEIGHT 10

class BridgeManager
{

    const size_t maxWeight;
    size_t actualWeight, nbCarWaiting, nbTruckWaiting;
    QMutex mutex;
    QWaitCondition cond;

public:
    BridgeManager(size_t maxWeight) : maxWeight(maxWeight), actualWeight(0), nbCarWaiting(0), nbTruckWaiting(0)
    {

    }

    void carAccess()
    {
        mutex.lock();
        if(actualWeight + CARWEIGHT > maxWeight){
            nbCarWaiting++;
            cond.wait(&mutex);
        }
        actualWeight += CARWEIGHT;
        mutex.unlock();
    }

    void truckAccess()
    {
        mutex.lock();

        if(actualWeight + TRUCKWEIGHT > maxWeight){
            nbTruckWaiting++;
            for(size_t i = 0; i < TRUCKWEIGHT; i++)
                cond.wait(&mutex);
        }

        actualWeight += TRUCKWEIGHT;
        mutex.unlock();
    }

    void carLeave()
    {
        mutex.lock();
        if(nbCarWaiting > 0){
            nbCarWaiting--;
            cond.wakeOne();
        }
        actualWeight -= CARWEIGHT;

        mutex.unlock();
    }

    void truckLeave()
    {
        mutex.lock();
        if(nbTruckWaiting > 0){
            nbTruckWaiting--;
            for(size_t i = 0; i < TRUCKWEIGHT; i++)
                cond.wakeOne();
        }
        actualWeight -= TRUCKWEIGHT;

        mutex.unlock();
    }
};
