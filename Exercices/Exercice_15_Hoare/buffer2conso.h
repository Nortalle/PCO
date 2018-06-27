#ifndef BUFFER2CONSO_H
#define BUFFER2CONSO_H

#include "abstractbuffer.h"


template<typename T> class Buffer2Conso : public AbstractBuffer<T> {
public:
    Buffer2Conso() {};

    virtual ~Buffer2Conso() {};

    virtual void put(T item) {Q_UNUSED(item)};
    virtual T get(void) {return T(0);};
};


#include <QSemaphore>

template<typename T> class Buffer2ConsoSemaphore : public AbstractBuffer<T> {
protected:

public:
    Buffer2ConsoSemaphore() {};

    virtual ~Buffer2ConsoSemaphore() {};

    virtual void put(T item) {};
    virtual T get(void) {};
};


#include <QSemaphore>

template<typename T> class Buffer2ConsoSemaphoreGeneral : public AbstractBuffer<T> {
protected:
    QSemaphore mutex;

public:
    Buffer2ConsoSemaphoreGeneral() {};

    virtual ~Buffer2ConsoSemaphoreGeneral() {};

    virtual void put(T item) {};
    virtual T get(void) {};
};

#include <QMutex>
#include <QWaitCondition>

template<typename T> class Buffer2ConsoMesa : public AbstractBuffer<T> {
protected:
    QMutex mutex;

public:
    Buffer2ConsoMesa() {};

    virtual ~Buffer2ConsoMesa() {};

    virtual void put(T item) {};
    virtual T get(void) {};
};


#include "hoaremonitor/hoaremonitor.h"

template<typename T> class Buffer2ConsoHoare : public AbstractBuffer<T>, public HoareMonitor {
protected:

public:
    Buffer2ConsoHoare() {};

    virtual ~Buffer2ConsoHoare() {};

    virtual void put(T item) {};
    virtual T get(void) {};
};


#endif // BUFFER2CONSO_H
