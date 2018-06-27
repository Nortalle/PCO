/**
 * PCO Exam 2015 YTA Ex. 1 Couleur
 * Warning: proposition de correction sans garantie !
 */

#include <QSemaphore>

enum {jaune = 0, noir, vert, bleu, NBCOULEUR} Couleur;

BarriereCouleur {
private:
	QSemaphore mutex;
	QSemaphore waitingForColor[];
	int nbWaitingForColor[];
public:
	BarriereCouleur() {
		waitingForColor = new QSemaphore[NBCOULEUR];
		nbWaitingForColor = new QSemaphore[NBCOULEUR];
	}

	~BarriereCouleur() {
		delete[] waitingForColor;
		delete[] nbWaitingForColor;
	}

	void acces (Couleur maCouleur) {
		mutex.acquire();
		++nbWaitingForColor[maCouleur]; // increment
		mutex.release();
		waitingForColor[maCouleur].acquire(); // atttente
	}

	void libere (Couleur couleurALiberer) {
		mutex.acquire();
		// liberate all those who are waiting for this color
		for (int i = 0; i < nbWaitingForColor[couleurALiberer]; ++i) {
			waitingForColor[couleurALiberer].release();
		}
		nbWaitingForColor[couleurALiberer] = 0; // reset
		mutex.release();
	}
}; // BarriereCouleur