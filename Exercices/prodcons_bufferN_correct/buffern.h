#ifndef BUFFERN_H
#define BUFFERN_H

#include "abstractbuffer.h"
#include <QSemaphore>

const int NoInitTamponN = 10;

template<typename T> class BufferNa : public AbstractBuffer<T> {

protected:
    T *elements;
    int writePointer;
    int readPointer;
    int bufferSize;
    QSemaphore mutex, waitNotFull, waitNotEmpty;

public:
    BufferNa(unsigned int size) : mutex(1), waitNotFull(size) {
        if ((elements = new T[size]) != 0) {
            writePointer = readPointer = 0;
            bufferSize = size;
            return;
        }
        throw NoInitTamponN;
    }

    virtual ~BufferNa() {}

    virtual void put(T item) {
        waitNotFull.acquire();
        mutex.acquire();
        elements[writePointer] = item;
        writePointer = (writePointer + 1) % bufferSize;
        waitNotEmpty.release();
        mutex.release();
    }

    virtual T get(void) {
        T item;
        waitNotEmpty.acquire();
        mutex.acquire();
        item = elements[readPointer];
        readPointer = (readPointer + 1) % bufferSize;
        waitNotFull.release();
        mutex.release();
        return item;
    }
};



#endif // BUFFERN_H
