#include <QSemaphore>

/** 
 * PCO 2015 Ex. 14 (c) YTA Yann Thoma
 * Coordination (Brigde Manager) with float numbers
 * Version: 1 (non-FIFO)
 * Objectif: ok (la synchronisation/section critique est garantie)
 * Commentaires:
 * 	- ordre FIFO pas respecté.
 * 	- famine possible des véhicules lourds
 *	  (toujours dépassés par des plus légers)
 */

class Vehicle {
	float weight; 
public:
	Vehicle(float weight) : weight(weight) {
		// equiv à this->weight = weight;
	} 
	float getWeight() const {
		return weight;
	}
	void stop() {
		// stopped
	}
	void start() {
		// started
	}
};

/***********************************************************/

class BridgeManagerFloat {
public:
	virtual void access(Vehicle *vehicle) = 0;
	virtual void leave(Vehicle *vehicle) = 0;
};

class BridgeManagerFloat1 : public BridgeManagerFloat {
	QSemaphore *mutex; // protège nbWaiting et currentWeight
	QSemaphore *waitingAccess; // blocage des accès/file d'attente
	int nbWaitingAccess; // nombre de vhc bloqués en attente
	float currentWeight; // poids sur le pont actuellement
	float maxWeight; // poids max du pont

public:
	BridgeManagerFloat1(float maxWeight): maxWeight(maxWeight) {
		// équiv à : this->maxWeight = maxWeight;
		currentWeight = nbWaitingAccess = 0;
		mutex = new QSemaphore(1);
		waitingAccess = new QSemaphore(0);
	}

	~BridgeManagerFloat1() {
		delete mutex;
		delete waitingAccess;
	}

	void access(Vehicle *vehicle) {
		// on garde trace si le vhc a été stopé, pour le restart
		bool stopped = false;
		mutex->acquire();
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
		mutex->release();
	}

	void leave(Vehicle *vehicle) {
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