#ifndef BUFFERN_H
#define BUFFERN_H

#include "abstractbuffer.h"
#include <semaphore.h>

const int NoInitTamponN = 10;

template<typename T>
class BufferN : public AbstractBuffer<T> {
public:

    BufferN(unsigned int size) {
        if (sem_init(&mutex,0,1) == 0)
            if (sem_init(&waitEmpty,0,0) == 0)
                if (sem_init(&waitFull,0,0) == 0)
                    if ((elements = new T[bufferSize]) != 0) {
                        writePointer = readPointer = nbElements = 0;
                        nbWaitingProd = nbWaitingCons = 0;
                        bufferSize = size;
                        return;
                    }
        // Exception
        throw NoInitTamponN;
    }

    ~BufferN() {
        sem_destroy(&mutex);
        sem_destroy(&waitEmpty);
        sem_destroy(&waitFull);
    }

    virtual void put(T item) {
        sem_wait(&mutex);
        if (nbElements == bufferSize) {
            nbWaitingProd += 1;
            sem_post(&mutex);
            sem_wait(&waitEmpty);
        }
        elements[writePointer] = item;
        writePointer = (writePointer + 1) % bufferSize;
        nbElements ++;
        if (nbWaitingCons > 0) {
            nbWaitingCons -= 1;
            sem_post(&waitFull);
        }
        else {
            sem_post(&mutex);
        }
    }

    virtual T get(void) {
        T item;
        sem_wait(&mutex);
        if (nbElements == 0) {
            nbWaitingCons += 1;
            sem_post(&mutex);
            sem_wait(&waitFull);
        }
        item = elements[readPointer];
        readPointer = (readPointer + 1) % bufferSize;
        nbElements --;
        if (nbWaitingProd > 0) {
            nbWaitingProd -= 1;
            sem_post(&waitEmpty);
        }
        else {
            sem_post(&mutex);
        }
        return item;
    }


protected:
    T *elements;
    int writePointer;
    int readPointer;
    int nbElements;
    int bufferSize;
    sem_t mutex, waitEmpty, waitFull;
    unsigned nbWaitingProd, nbWaitingCons;
};







template<typename T>
class BufferNa : public AbstractBuffer<T> {
public:

    BufferNa(unsigned int size) {
        if (sem_init(&mutex,0,1) == 0)
            if (sem_init(&waitNotFull,0,size) == 0)
                if (sem_init(&waitNotEmpty,0,0) == 0)
                    if ((elements = new T[bufferSize]) != 0) {
                        writePointer = readPointer = 0;
                        bufferSize = size;
                        return;
                    }
        // Exception
        throw NoInitTamponN;
    }

    ~BufferNa() {
        sem_destroy(&mutex);
        sem_destroy(&waitNotFull);
        sem_destroy(&waitNotEmpty);
    }

    virtual void put(T item) {
        sem_wait(&waitNotFull);
        sem_wait(&mutex);
        elements[writePointer] = item;
        writePointer = (writePointer + 1) % bufferSize;
        sem_post(&waitNotEmpty);
        sem_post(&mutex);
    }

    virtual T get(void) {
        T item;
        sem_wait(&waitNotEmpty);
        sem_wait(&mutex);
        item = elements[readPointer];
        readPointer = (readPointer + 1) % bufferSize;
        sem_post(&waitNotFull);
        sem_post(&mutex);
        return item;
    }


protected:
    T *elements;
    int writePointer;
    int readPointer;
    int bufferSize;
    sem_t mutex, waitNotFull, waitNotEmpty;
};

#endif // BUFFERN_H
