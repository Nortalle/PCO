#ifndef BUFFER1_H
#define BUFFER1_H

#include <semaphore.h>

#include "abstractbuffer.h"

#define Vide false
#define Plein true

const int NoInitTamponSimple = 10;

template<typename T>
class Buffer1 : public AbstractBuffer<T> {
public:

    Buffer1() {
        if (sem_init(&mutex,0,1) == 0)
            if (sem_init(&waitEmpty,0,0) == 0)
                if (sem_init(&waitFull,0,0) == 0) {
                    state = Vide;
                    nbWaitingProd = nbWaitingCons = 0;
                    return;
                }
        // Exception
        throw NoInitTamponSimple;
    }

    ~Buffer1() {
        sem_destroy(&mutex);
        sem_destroy(&waitEmpty);
        sem_destroy(&waitFull);
    }

    virtual void put(T item) {
        sem_wait(&mutex);
        if (state == Plein) {
            nbWaitingProd += 1;
            sem_post(&mutex);
            sem_wait(&waitEmpty);
        }
        element = item;
        if (nbWaitingCons > 0) {
            nbWaitingCons -= 1;
            sem_post(&waitFull);
        }
        else {
            state = Plein;
            sem_post(&mutex);
        }
    }

    virtual T get(void) {
        T item;
        sem_wait(&mutex);
        if (state == Vide) {
            nbWaitingCons += 1;
            sem_post(&mutex);
            sem_wait(&waitFull);
        }
        item = element;
        if (nbWaitingProd > 0) {
            nbWaitingProd -= 1;
            sem_post(&waitEmpty);
        }
        else {
            state = Vide;
            sem_post(&mutex);
        }
        return item;
    }

protected:
    T element;
    sem_t mutex, waitEmpty, waitFull;
    bool state;
    unsigned nbWaitingProd, nbWaitingCons;
};



template<typename T>
class Buffer1a : public AbstractBuffer<T> {
public:

    Buffer1a() {
        if (sem_init(&waitEmpty,0,1) == 0)
            if (sem_init(&waitFull,0,0) == 0) {
                return;
            }
        // Exception
        throw NoInitTamponSimple;
    }

    ~Buffer1a() {
        sem_destroy(&waitEmpty);
        sem_destroy(&waitFull);
    }

    virtual void put(T item) {
        sem_wait(&waitEmpty);
        element = item;
        sem_post(&waitFull);
    }

    virtual T get(void) {
        T item;
        sem_wait(&waitFull);
        item = element;
        sem_post(&waitEmpty);
        return item;
    }

protected:
    T element;
    sem_t waitEmpty, waitFull;
};
#endif // BUFFER1_H
