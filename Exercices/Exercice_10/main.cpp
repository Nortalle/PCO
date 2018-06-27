
#include <QThread>

#include "pcomuter_qt.h"



#include <QThread>
#include <QMutex>
#include <iostream>


static int global = 0;
static int iterations = 1000000;
static PcoMutex mutex;

class CounterThread: public QThread
{
public:
     void run() Q_DECL_OVERRIDE {
        for(int i=0;i<iterations;i++) {
            //if(i%1000 == 0)
             //   std::cout << i << std::endl;
            mutex.lock();

            global = global + 1;

            mutex.unlock();

        }
    }
};

int main(int argc, char *argv[])
{

    CounterThread threads[2];
    for(int i=0;i<2;i++)
        threads[i].start();
    for(int i=0;i<2;i++)
        threads[i].wait();

    std::cout << "Fin des taches : global = " << global
              << " (" << 2*iterations << ")" << std::endl;

    return 0;
}
