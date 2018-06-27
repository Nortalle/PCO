
#include <QSemaphore>

#define CARWEIGHT 1
#define TRUCKWEIGHT 10
class BridgeManager
{
    const size_t maxWeight;
    size_t actualWeight, nbCarWaiting, nbTruckWaiting;
    QSemaphore mutex, bridge;


public:

    BridgeManager(size_t maxWeight) : maxWeight(maxWeight), actualWeight(0), mutex(1), bridge(0), nbCarWaiting(0), nbTruckWaiting(0)
    {

    }

    void carAccess()
    {
        mutex.acquire();
        if(actualWeight + CARWEIGHT <= maxWeight){
            actualWeight += CARWEIGHT;
            mutex.release();
        } else {
            nbCarWaiting++;
            mutex.release();
            bridge.acquire();
            actualWeight += CARWEIGHT;
        }
    }

    void truckAccess()
    {
        mutex.acquire();
        if(actualWeight + TRUCKWEIGHT <= maxWeight){
            actualWeight += TRUCKWEIGHT;
            mutex.release();
        } else {
            nbTruckWaiting++;
            mutex.release();
                bridge.acquire(TRUCKWEIGHT);

            actualWeight += TRUCKWEIGHT;
        }
        mutex.release();

    }

    void carLeave()
    {
        mutex.acquire();
        if(nbCarWaiting > 0){
            nbCarWaiting--;
            bridge.release();
        }
        actualWeight -= CARWEIGHT;

        mutex.release();
    }

    void truckLeave()
    {
        mutex.acquire();
        if(nbTruckWaiting > 0){
            nbTruckWaiting--;

                bridge.release(TRUCKWEIGHT);
        }
        actualWeight -= TRUCKWEIGHT;

        mutex.release();
    }
};
