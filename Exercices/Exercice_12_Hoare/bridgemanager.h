#include <QSemaphore>
#include "hoaremonitor.h"

#define CARWEIGHT 1
#define TRUCKWEIGHT 10

class BridgeManager : public HoareMonitor
{
    const size_t maxWeight;
    size_t actualWeight, nbCarWaiting, nbTruckWaiting;
    Condition cond;

public:
    BridgeManager(size_t maxWeight) : maxWeight(maxWeight), actualWeight(0), nbCarWaiting(0), nbTruckWaiting(0)
    {

    }

    void carAccess()
    {
        monitorIn();

        if(actualWeight + CARWEIGHT > maxWeight){
            MoniWait(cond);
        }

        actualWeight += CARWEIGHT;
        monitorOut();
    }

    void truckAccess()
    {
        monitorIn();

        while(actualWeight + TRUCKWEIGHT > maxWeight){
            MoniWait(cond);
        }

        actualWeight += TRUCKWEIGHT;
        monitorOut();

    }

    void carLeave()
    {
        monitorIn();
        actualWeight -= CARWEIGHT;
        signal(cond);

        monitorOut();

    }

    void truckLeave()
    {
        monitorIn();

        actualWeight -= TRUCKWEIGHT;
        signal(cond);

        monitorOut();
    }
};
