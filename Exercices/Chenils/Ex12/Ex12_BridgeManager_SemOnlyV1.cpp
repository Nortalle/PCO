/** 
 * PCO 2015 Ex. 12 (c) YTA Yann Thoma
 * Coordination (Brigde Manager) 
 * Version: semaphore-only, version 1.
 * Objectif: NON respecté
 * Commentaire: 1/10 de camion peut passer le pont.
 * Interblocage possible des camions.
 */

#define TRUCK_WEIGHT 10
#define CAR_WEIGHT 1

class BridgeManager {
	private:
		// blocage/file d'attente pour les tonnes supplémentaires
		QSemaphore* counter;
	public:
		BridgeManager(unsigned int maxWeight) {
			counter = new QSemaphore(maxWeight);
		}

		~BridgeManager() {
			delete counter;
		}

		void carAccess() {
			acquire(CAR_WEIGHT);
		}

		void carLeave() {
			release(TRUCK_WEIGHT);
		}

		void truckAccess() {
			acquire(CAR_WEIGHT);
		}

		void truckLeave() {
			release(TRUCK_WEIGHT);
		}

	protected:
		void acquire(unsigned int weight) {
			for(int i = 0; i < weight; i++) {
				counter->acquire();
			}
		}
		
		void release(unsigned int weight) {
			for(int i = 0; i < weight; i++) {
				counter->release();
			}
		}
};