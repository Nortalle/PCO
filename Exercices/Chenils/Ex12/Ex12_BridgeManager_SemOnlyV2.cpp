/** 
 * PCO 2015 Ex. 12 (c) YTA Yann Thoma
 * Coordination (Brigde Manager) 
 * Version: semaphore-only, version 2.
 * Objectif: respecté, fonctionne selon le cahier des charges
 * Commentaire: solution simple mais pas flexible. 
 * OK ici car poids entiers et relativement petits.
 * Mais... poids en kg (grands nombres): non-scalable, dégradation.
 * Mais... poids en float: impossible.
 */

#define TRUCK_WEIGHT 10
#define CAR_WEIGHT 1

class BridgeManager{
	private:
		QSemaphore* counter; // file d'attente pour les tonnes supplémentaires
		QSemaphore* fifo;  // oblige les vhc a passer entièrement
	public:
		BridgeManager(unsigned int maxWeight) {
			counter = new QSemaphore(maxWeight);
			fifo = new QSemaphore(1);
		}
		~BridgeManager() {
			delete counter;
			delete fifo;
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
			// fifo: un à la fois - une fois qu'un vhc s'est engagé il doit terminer
			fifo->acquire();
			for(int i = 0; i < weight; i++) {
				counter->acquire();
			}
			fifo->release();
		}
		void release(unsigned int weight) {
			for(int i = 0; i < weight; i++) {
				counter->release(); 
			} // le release n'a pas besoin d'être protégé par le fifo
		}
};