
#include <QSemaphore>

class PcoBarrier
{
    QSemaphore mutex;
    QSemaphore attente;
    size_t nbToWait;
    size_t nbWaiting;

public:
    PcoBarrier(size_t nbToWait) :  nbToWait(nbToWait), nbWaiting(0),mutex(1), attente(0)
    {
    }

    ~PcoBarrier()
    {
    }

    void wait()
    {
        mutex.acquire();
        nbWaiting++;
        if(nbWaiting == nbToWait){
            mutex.release();
            for(size_t i = 0; i < nbToWait-1; i++){
                attente.release();
            }

        } else {
            mutex.release();
            attente.acquire();
        }
    }
};
