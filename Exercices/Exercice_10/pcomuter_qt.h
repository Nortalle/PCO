#ifndef PCOMUTER_QT_H
#define PCOMUTER_QT_H
#include <QSemaphore>
#include <iostream>


class PcoMutex
{
    QSemaphore* mutex;
    QSemaphore* attente;

    int nbThreadWaiting;
    bool occupied;
public:
    PcoMutex() : occupied(false), nbThreadWaiting(0), mutex(1), attente(0)
    {

    }

    ~PcoMutex()
    {
        delete attente;
        delete mutex;
    }

    void lock()
    {
        mutex->acquire();

        if(occupied){

            nbThreadWaiting++;
            mutex->release();
            attente->acquire();
        }else{
            occupied = true;
            mutex->release();
        }
    }

    void unlock()
    {
        mutex->acquire();
        if(occupied){
            if (nbThreadWaiting>0) {
                nbThreadWaiting--;
                attente->release();
            }
            else {
                occupied = false;
            }
        }
        mutex->release();
    }

    bool trylock()
    {
        mutex->acquire();
        if (occupied) {
            mutex->release();
            return false;
        }
        else {
            occupied = true;
            mutex->release();
            return true;
        }
    }  //! Returns true if the mutex can be acquired, false if it is already locked
};


#endif // PCOMUTER_QT_H
