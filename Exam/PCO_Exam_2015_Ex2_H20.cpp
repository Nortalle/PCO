/**
 * PCO Exam 2015 YTA Ex. 2 Barriere H20
 * Warning: proposition de correction sans garantie !
 */
#include <QSemaphore>

/*
* 3 ordres posssibles d'arrivée:
1. HHO -> l'oxygène libérera les deux hydrogène
2. HOH -> le 1er hydro bloque, l'oxygène bloque, le 2e hydro les libère les deux
3. OHH -> idem 2.
*/

BarriereEau {
private:
	QSemaphore mutex;
	QSemaphore oxygen;
	int nbOxygen;
	QSemaphore hydrogen;
	int nbHydrogen;
public:
	void accesOxygene() {
		mutex.acquire();
		++nbOxygen;
		if (nbHydrogen < 2) { // CAS 2 & 3
			// il n'y pas encore 2 hydrogène quand l'oxygène arrive:
			// on attend le DEUXIEME hydrogène (qui liberera)
			mutex.release();
			oxygen.acquire(); // mutex reception
		} else { // nbHydrogen >= 2 - CAS 1
			// il y a DEJA 2 hydrogène quand l'oxygène arrive:
			// on les libère (ils vont se détruire) et c'est fini
			--nbOxygen;
			nbHydrogen -= 2; // reset pour tous (je suis le seul a avoir la section critique)
			hydrogen.release();
			hydrogen.release();
			mutex.release();
		}
		// it's magic, i'm now water
	}
	void accesHydrogene() {
		mutex.acquire();
		++nbHydrogen;
		if (nbOxygen >= 1 && nbHydrogen >= 2) { // CAS 2 & 3 (deuxième H)
			// il y déjà l'oxygène et le premier hydrogène
			// je réveille l'autre hydrogène et l'oxygène 
			--nbOxygen;
			nbHydrogen -=2; // reset pour tous (je suis le seul a avoir la section critique)
			hydrogen.release(); // liberation du 1er hydrogene
			oxygen.release(); // liberation de l'oxygène
			mutex.release();
		} else { // tous les case pour le premier H, CAS 1 pour le deuxième
			// Il n'y a pas encore d'oxygène et 2 hydrogène: j'attend
			// arrivera TOUJOURS pour le premier hydrogène
			// arrivera uniquement dans le cas 1 pour le pour le deuxième
			// dans le cas 1, c'est l'oxygène qui réveille les deux hydrogène
			// cas 2 et 3, c'est le deuxième hydrogène qui réveille le premier et l'oxygène
			mutex.release();
			hydrogen.acquire();
		}
		QThread::exit(0); // destruction du thread
	}
}; // BarriereEau