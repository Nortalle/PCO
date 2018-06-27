/** 
 * PCO 2015 Ex. 11 (c) YTA Yann Thoma
 * Barrière de synchronisation (rendez-vous de N threads)
 * Objectif: réalisé. Commentaire: solution optimale.
 */

class PcoBarrier{

private:

	const unsigned int nbToWait; // nombre max à attendre 
	unsigned int waiting = 0; // nombre en attente actuellement
	QSemaphore* mutex; // protège waiting
	QSemaphore* attente; // bloque/file d'attente
	
public:

	PcoBarrier(unsigned int nbToWait): nbToWait(nbToWait) {
		// équiv à: this.nbToWait = nbToWait; mais const !
		mutex = new QSemaphore(1); // ouvert
		attente = new QSemaphore(0); // fermé
	}

	~PcoBarrier() {
		delete mutex;
		delete attente;
	}

	void wait() {
		mutex->aquire();
		waiting++;

		if (waiting == nbToWait) {
			// libération de tous les autres en attente 
			// !! ne pas compter soi-même et revenir à l'état initial
			for(; waiting > 1; waiting--) {
				attente->release();
			}
			waiting = 0;
			mutex->release();
		} else {
			// waiting < nbToWait: mise en attente
			mutex->release();
			attente->aquire();
		}
	}
}; // fin de la clase PcoBarrier