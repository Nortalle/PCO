

#include <iostream>

#include <QThread>

#include "pcobarrier.h"



static PcoBarrier *barrier;

class CounterThread: public QThread
{
public:
    void setId(int id) {
        this->id = id;
    }

    void run() Q_DECL_OVERRIDE {
        std::cout << "Thread " << id << " arriving" << std::endl;
        barrier->wait();
        std::cout << "Thread " << id << " leaving" << std::endl;
    }

protected:
    int id;
};

#define NB_THREADS 4

int main(int /*argc*/, char */*argv*/[])
{
    CounterThread threads[NB_THREADS];
    barrier = new PcoBarrier(NB_THREADS);
    for(int i = 0; i < NB_THREADS; i++) {
        threads[i].setId(i);
        threads[i].start();
    }
    for(int i = 0; i < NB_THREADS; i++)
        threads[i].wait();

    delete barrier;

    return 0;
}
