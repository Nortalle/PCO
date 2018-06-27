#ifndef BUS_TE1_2015_SEMAPHORE
#define BUS_TE1_2015_SEMAPHORE

#define BUS_MAX 10 // bus capacity / capacité d'un bus
#define PERS_MAX 30 // max nb people waiting/ nombre max de personnes en attente

#include <QSemaphore>

/** 
 * HEIG-TIC-PCO TE1 2015 Ex.3 (c) YTA Yann Thoma
 * Implemenation: with QSemaphore only
 * Idem seulement avec QSemaphore.
 * Author: Valentin Minder */
class BusStopSemaphore : public BusStop {
private:
    QSemaphore* waitForBus; // blocks waiting people / bloque les personnes en attente
    QSemaphore* mutex; // protège/protects nbPeopleWaiting
    int nbPeopleWaiting; // nb people waiting / nombre de personnes en attente

public:
    BusStop():{
        waitForBus = new QSemaphore(0); // closed/fermé
        mutex = new QSemaphore(1); // open/ouvert
        nbPeopleWaiting = 0;
    }

    virtual ~BusStop(){
        delete waitForBus;
        delete mutex;
    }

    virtual void peopleArrives(){
        mutex->acquire();
        if(nbPeopleWaiting > PERS_MAX){
            // the person goes away / la personne passe son chemin
            mutex->release();
        } else {
            // the person blocks and waits for the bus / la personne attend le bus
            nbPeopleWaiting++;
            mutex->release(); // restitution du mutex / mutex is given back
            waitForBus->acquire(); // blocage: mise en attente / blocks
        }
    }

    virtual void busArrives(){
        mutex->acquire();
        // liberation d'au max BUS_MAX personnes 
        // à condition qu'il y en ait assez (nbPeopleWaiting)
        // liberation of max BUS_MAX people, if they are there
        // (with Semaphore, its very important that we liberate the exact actual number of people
        // otherwise it will open too much for next arrivals.
        int toload = min(BUS_MAX, nbPeopleWaiting);
        nbPeopleWaiting -= toload;
        int i;
        for(i = 0; i < toload; ++i) {
           waitForBus->release(); // liberation
        }
        mutex->release();
    }
}; // class BusStopSemaphore

#endif // BUS_TE1_2015_SEMAPHORE