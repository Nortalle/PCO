#include <QSemaphore>

/** 
 * PCO 2015 Ex. 14 (c) YTA Yann Thoma
 * Coordination (Brigde Manager) with float numbers
 * Version: 2 (FIFO - les modifications sont marquées "NEW")
 * Objectif: ok (la synchronisation/section critique est garantie)
 * Commentaires:
 * 	- ordre FIFO EST respecté
 * 	- pas de famine (passage dans l'ordre)
 *	- implémentation très proche du concept de moniteur
 */

class BridgeManagerFloat2 : public BridgeManagerFloat {
	QSemaphore *mutex; // protège nbWaiting et currentWeight
	QSemaphore *waitingAccess; // blocage des accès/file d'attente
	unsigned int nbWaitingAccess; // nombre de vhc bloqués en attente
	float currentWeight; // poids sur le pont actuellement
	float maxWeight; // poids max du pont

	// NEW FIFO
	QSemaphore *waitingFifo; // blocage en ordre FIFO
	unsigned int nbWaitingFifo; // nombre de vhc en attente de passage
	boolean accessing;

public:
	BridgeManagerFloat1(float maxWeight): maxWeight(maxWeight) {
		// équiv à : this->maxWeight = maxWeight;
		currentWeight = nbWaitingAccess = 0;
		mutex = new QSemaphore(1);
		waitingAccess = new QSemaphore(0);

		// NEW FIFO
		waitingFifo = new QSemaphore(0);
		nbWaitingFifo = 0;
        accessing = false;
	}

	~BridgeManagerFloat1() {
		delete mutex;
		delete waitingAccess;
		delete waitingFifo; // NEW
	}

	void access(Vehicle *vehicle) {
		// on garde trace si le vhc a été stopé, pour le restart
		bool stopped = false;
		mutex->acquire();

		// NEW FIFO: le premier passe. Les suivants bloquent.
		if (accessing) { // nota: il y avait un while, mais je pense que if suffit.
			nbWaitingFifo ++;
			mutex->release();
			waitingFifo->acquire(); // mutex transmis
		}
		accessing = true;

		// tant qu'il n'est pas possible d'accéder au pont...
		while (currentWeight + vehicle->getWeight() > maxWeight) { 
			nbWaitingAccess++;
			mutex->release(); // relâchement: un autre pourra passer ou partir
			// si pas déjà arrêté, on arrête (éviter les start/stop)
			if (!stopped) {
				vehicle->stop();
				stopped = true;
			}
			waitingAccess->acquire();
			// mutex->acquire() pas nécessaire car transmission de mutex!
		}
		currentWeight += vehicle->getWeight();
		// s'il a été arreté, restart
		if (stopped) {
			vehicle->start();
		}

		// NEW FIFO: on quitte: on libère le suivant dans le fifo s'il existe
		accessing = false;
		if (nbWaitingFifo != 0) {
			nbWaitingFifo--;
			waitingFifo->release(); // transmission de mutex.
		} else {
			mutex->release();
		}
	}

	void leave(Vehicle *vehicle) {
		// NEW FIFO: pas de fifo nécessaire pour le leave

		mutex->acquire();
		currentWeight -= vehicle->getWeight();
		
		// relâchement éventuel d'un vhc en attente.
		if (nbWaitingAccess > 0) {
			nbWaitingAccess--;
			waitingAccess->release();
			// transmission de mutex / section critique
		} else {
			mutex->release();
		}

		// sans transmission de mutex: mutex->release(); ici au lieu du esle
	}
};