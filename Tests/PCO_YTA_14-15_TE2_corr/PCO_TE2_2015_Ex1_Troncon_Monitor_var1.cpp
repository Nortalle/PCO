#include <QCoreApplication>

#include <QMutex>
#include <QWaitCondition>

/** 
 * HEIG-TIC-PCO TE2 2015 Ex.1 (c) YTA Yann Thoma
 * Implementation: as a Monitor, with QWaitCondition and QMutex.
 * Version: student proposition, NOT verified, to avoid:
 *	- if maxTrams == 1, does the correct thing (no deadlock, and current direction liberated)
 *  - for liberation when changing direction (and no more wake of next tram when accessing)
 * NOT VERIFIED
 */
class Troncon {
protected:
	QMutex mutex; // protects all critic variables
	QWaitCondition attente[2]; // wait condition for each direction
	int nbWaiting[2];
	int maxTrams; // nb max of tram on the critic segment
	int sensCourant; // current direction
	int nbTrams; // currrent nb of trams on the segment
	
public:
	Troncon(int maxTrams) : maxTrams(maxTrams), nbTrams(0) { }
	
	void acces(int sens)
	{
		mutex.lock();
		
		/* we can access the railway if
		 * - there is room for a tram AND
		 * - the direction is the same OR no more trams are running in the segment
		 */
		++nbWaiting[sens];
		while((nbTrams >= maxTrams) || ((nbTrams > 0) && sensCourant != sens)) {
			attente[sens].wait(&mutex);
		}
		--nbWaiting[sens];
		
		nbTrams++;
		sensCourant = sens;

		/* // alternative au for dans la libération
		if (nbWaiting[sens] != 0) {
			attente[sens].wakeOne();
		} */
		mutex.unlock();
	}
	
	void sortie(int sens) {
		mutex.lock();
		nbTrams--;
		if (nbWaiting[sens] != 0) {
			// we always, eventually wake up another tram in the same direction
			attente[sens].wakeOne();
		} else if (nbTrams == 0 && nbWaiting[1-sens] != 0) {
			// if we are the last tram
			// we switch direction and we wake up another tram in the opposite direction
			sensCourant = 1 - sensCourant;
			/* ou seulement ceci, avec l'alternative ci-dessus
			attente[1 - sens].wakeOne();
			*/
			int toLiberate = min (nbWaiting, maxTrams);
			int i;
			for (i = 0; i < toLiberate; ++i) {
				attente[1 - sens].wakeOne();
			}
		}
		mutex.unlock();
	}
};