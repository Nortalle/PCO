
#include "abstractbuffer.h"

#include <QSemaphore>

template<typename T> class Buffer2Conso : public AbstractBuffer<T> {
private:
    T *elements;
    int writePointer;
    int readPointer;
    int nbElements;
    const int bufferSize;
    QSemaphore mutex, waitEmpty, waitFull;
    size_t nbWaitingProd, nbWaitingCons;

public:
    Buffer2Conso(size_t size) :bufferSize(size),
        mutex(1),
        waitEmpty(0),
        waitFull(1),
        elements(new T(bufferSize)),
        writePointer(0),
        readPointer(0),
        nbElements(0),
        nbWaitingCons(0),nbWaitingProd(0){}

    virtual ~Buffer2Conso() {
        delete[] elements;
    }

    virtual void put(T item) {
        mutex.acquire();
        if(nbElements == bufferSize){
            nbWaitingProd++;
            mutex.release();
            waitEmpty.acquire();
        }
        elements[writePointer] = item;
        writePointer = (writePointer +1) % bufferSize;
        nbElements++;
        if(nbWaitingCons > 0){
            nbWaitingCons--;
            waitFull.release();
        } else {
            mutex.release();
        }
    }

    virtual T get(void) {
        T item;
        mutex.acquire();
        if(nbElements == 0){
            nbWaitingCons++;
            mutex.release();
            waitFull.acquire();
        }
        item = elements[readPointer];
        readPointer = (readPointer + 1) % bufferSize;
        nbElements--;
        if(nbWaitingProd > 0){
            nbWaitingProd--;
            waitEmpty.release();
        } else {
            mutex.release();
        }
        return item;
    }
};
