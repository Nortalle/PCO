#ifndef BUS_TE1_2015_MONITOR
#define BUS_TE1_2015_MONITOR

#define BUS_MAX 10 // bus capacity / capacité d'un bus
#define PERS_MAX 30 // max nb people waiting / nombre max de personnes en attente

#include <QMutex>
#include <QWaitCondition>

/** 
 * HEIG-TIC-PCO TE1 2015 Ex.3 (c) YTA Yann Thoma
 * Implemenation: as a Monitor, with QWaitCondition and QMutex.
 * Idem avec des QWaitCondition plutôt que QSemaphore.
 * Author: Valentin Minder */
class BusStopMonitor : public BusStop {

protected:
	QwaitCondition* waitForBus; // blocks waiting people / bloque les personnes en attente
	Qmutex* mutex; // protège/protects nbPeopleWaiting
	int nbPeopleWaiting; // nb people waiting / nombre de personnes en attente

public:
	BusStopMonitor() {
		mutex = new QMutex();
		waitForBus = new QWaitCondition();
		nbPeopleWaiting = 0;
	}

	virtual ~BusStopMonitor() {
		delete mutex;
		delete waitForBus;
	}

	virtual void peopleArrives() {
		mutex.lock()
		if(nbPeopleWaiting > PERS_MAX) {
			// the person goes away / la personne passe son chemin
			mutex.unlock();
		} else {
			// the person blocks and waits for the bus / la personne attend le bus
			++nbPeopleWaiting;
			waitForBus.wait(mutex); // blocage: mise en attente / blocks
			mutex.unlock(); // restitution du mutex / mutex is given back
		}
	}
	
	virtual void busArives() {
		mutex.lock();
        // liberation d'au max BUS_MAX personnes
        // à condition qu'il y en ait assez (nbPeopleWaiting)
        // liberation of max BUS_MAX people, if they are there
        // (with Monitor, its not very relevant if we liberate 
        // more than the number of actual people waiting, it will have no effect)
		int toload = min(BUS_MAX, nbPeopleWaiting);
		nbPeopleWaiting -= toload;
		int i;
		for(i = 0; i < toload; ++i) {
			waitForBus.wakeOne(); // libération
		}
		mutex.unlock();
	}
}; // classe BusStopMonitor
#endif // BUS_TE1_2015_MONITOR