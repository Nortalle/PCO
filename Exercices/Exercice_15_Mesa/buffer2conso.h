#ifndef BUFFER2CONSO_H
#define BUFFER2CONSO_H

#include "abstractbuffer.h"


#include <QMutex>
#include <QWaitCondition>

template<typename T> class Buffer2ConsoMesa : public AbstractBuffer<T> {
protected:
    QMutex mutex;
    T* elements;
    size_t writerPointer, readerPointer, nbWaitingProd, nbElements;
    const size_t size;
    QWaitCondition notEmpty, notFull;

public:
    Buffer2ConsoMesa(size_t size) : size(size),
        elements(new T(size)),
        writerPointer(0),
        readerPointer(0),
        nbWaitingProd(0),
        nbElements(0)
    {}

    virtual ~Buffer2ConsoMesa() {
        delete[] elements;
    }

    virtual void put(T item) {
        mutex.lock();
        if(nbElements == size){
            nbWaitingProd++;
            notFull.wait(&mutex);
        }
        elements[writerPointer] = item;
        writerPointer = (writerPointer + 1) % size;
        nbElements++;
        notEmpty.wakeOne();
        mutex.unlock();
    }

    virtual T get(void) {
        T item;
        mutex.lock();
        notEmpty.wait(&mutex);
        item = elements[readerPointer];
        readerPointer = (readerPointer + 1) % size;
        nbElements--;
        if(nbWaitingProd > 0){
            nbWaitingProd--;
            notFull.wakeOne();
        }
        mutex.unlock();
        return item;
    }
};



#endif // BUFFER2CONSO_H
