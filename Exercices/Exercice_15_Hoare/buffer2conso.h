#ifndef BUFFER2CONSO_H
#define BUFFER2CONSO_H

#include "abstractbuffer.h"

#include "hoaremonitor/hoaremonitor.h"

template<typename T> class Buffer2ConsoHoare : public AbstractBuffer<T>, public HoareMonitor {
protected:

    T* elements;
    size_t writerPointer, readerPointer, nbWaitingProd, nbElements;
    const size_t size;
    Condition notEmpty, notFull;

public:
    Buffer2ConsoHoare(size_t size) : size(size),
        elements(new T(size)),
        writerPointer(0),
        readerPointer(0),
        nbWaitingProd(0),
        nbElements(0)
    {}

    virtual ~Buffer2ConsoHoare() {
        delete[] elements;
    }

    virtual void put(T item) {
        monitorIn();
        if(nbElements == size){
            nbWaitingProd++;
            wait(notFull);
        }
        elements[writerPointer] = item;
        writerPointer = (writerPointer + 1) % size;
        nbElements++;
        signal(notEmpty);
        monitorOut();
    }
    virtual T get(void) {
        T item;
        monitorIn();
        wait(notEmpty);
        item = elements[readerPointer];
        readerPointer = (readerPointer + 1) % size;
        nbElements--;
        if(nbWaitingProd > 0){
            nbWaitingProd--;
            signal(notFull);
        }
        monitorOut();
        return item;
    }
};


#endif // BUFFER2CONSO_H
