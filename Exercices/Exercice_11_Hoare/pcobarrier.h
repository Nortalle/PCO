
#include <QSemaphore>
#include "hoaremonitor.h"

class PcoBarrier : public HoareMonitor
{

    Condition cond;
    size_t nbToWait;
    size_t nbWaiting;

public:
    PcoBarrier(unsigned int nbToWait) : nbToWait(nbToWait), nbWaiting(0)
    {
    }

    ~PcoBarrier()
    {
    }

    void wait()
    {
        monitorIn();
        nbWaiting++;
        if((nbToWait )== nbWaiting){
            for(int i = 0; i < nbToWait -1; i++)
                signal(cond);
        } else {
            MoniWait(cond);
        }

        monitorOut();
    }
};
