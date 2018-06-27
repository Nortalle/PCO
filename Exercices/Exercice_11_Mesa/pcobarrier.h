
#include <QMutex>
#include <QWaitCondition>

class PcoBarrier
{
    QMutex mutex;
    QWaitCondition cond;
    size_t nbToWait;
    unsigned int nbWaiting;


public:
    PcoBarrier(size_t nbToWait): nbToWait(nbToWait), nbWaiting(0)
    {
    }

    ~PcoBarrier()
    {
    }

    void wait()
    {
        mutex.lock();
        nbWaiting++;
        if(nbWaiting == nbToWait){
            cond.wakeAll();

        } else {
            cond.wait(&mutex);
        }

        mutex.unlock();
    }
};
