#ifndef CORRIGER_TE2
#define CORRIGER_TE2

#include <QMutex>
#include <QWaitCondition>
#include <QSemaphore>

// Exercice 1 =====================================================
class Troncon{
private:
    QMutex mutex;
    QWaitCondition attente[2];
    int maxTrams;
    int sensCourant;
    int nbTrams;
public:
    Troncon(int maxTrams): maxTrams(maxTrams), nbTrams(0){}

    void acces(int sens){
        mutex.lock();
        while((nbTrams >= maxTrams) || ((nbTrams > 0) && (sensCourant != sens))){
            attente[sens].wait(&mutex);
        }
        nbTrams++;
        sensCourant = sens;
        attente[sens].wakeOne();
        mutex.unlock();
    }

    void sortie(int sens){
        mutex.lock();
        nbTrams--;
        if(nbTrams == 0){
            sensCourant = 1-sensCourant;
            attente[1-sens].wakeOne();
        }else{
            attente[sens].wakeOne();
        }
        mutex.unlock();
    }
};

// Exercice 2 =====================================================
class BarriereAB{
private:
    QMutex mutex;
    QWaitCondition cond[2];
    int nb[2];
    int size;

public:
    BarriereAB(int n) : size(n){
        nb[0] = 0;
        nb[1] = 0;
    }

    void arrive(int classe){
        mutex.lock();
        nb[classe] ++;
        if(nb[classe] + nb[classe[1-classe] < size]){
            cond[classe].wait(&mutex);
        }
        nb[classe]--;
        if(nb[classe] > 0){
            cond[classe].wakeOne();
        }else if(nb[1-classe] > 0){
            cond[1-classe].wakeOne();
        }
        mutex.unlock();
    }
};

// Exercice 3 =====================================================
typedef enum {Pasta = 0, Rice = 1, Tomate = 2, NBFOODTYPES = 3} FoodType;

typedef struct{
    FoodType type;
} Food;

class FoodBuffer{
private:
    QSemaphore mutex;
    QSemaphore waitProd;
    int prodWaiting;
    QSemaphore waitCons[NBFOODTYPES];
    int consWaiting[NBFOODTYPES];

protected:
    Food *array;
    int size;
    int writerPointer, readPointer, nbTot;

public:
    FoodBuffer(int size): size(size), writerPointer(0), readPointer(0), nbTot(0){
        mutex.release();
        array = new Food;
        prodWaiting = 0;
        for(int i = 0; i < NBFOODTYPES; i++){
            consWaiting[i] = 0;
        }
    }

    ~FoodBuffer(){
        delete[] array;
    }

    void put(Food food){
        mutex.acquire();
        if(nbTot == size){
            prodWaiting ++;
            mutex.release();
            waitProd.acquire();
        }
        array[writerPointer] = food;
        writerPointer = (writerPointer + 1)%size;
        nbTot ++;
        if(consWaiting[array[readPointer].type] > 0){
            consWaiting[food.type] --;
            waitCons[food.type].release();
        }else{
            mutex.release();
        }
    }

    Food get(FoodType type){
        Food result;
        mutex.acquire();
        if((nbTot == 0) || (array[readPointer].type != type)){
            consWaiting[type]++;
            mutex.release();
            waitCons[type].acquire();
        }
        result = array[readPointer];
        readPointer = (readPointer + 1) % size;
        nbTot --;
        if(consWaiting[array[readPointer].type] > 0){
            consWaiting[food.type] --;
            waitCons[food.type].release();
        }else if(prodWaiting > 0){
            prodWaiting --;
            waitProd.release();
        }else{
            mutex.release();
        }
        return result;

    }
};

#endif // CORRIGER_TE2

