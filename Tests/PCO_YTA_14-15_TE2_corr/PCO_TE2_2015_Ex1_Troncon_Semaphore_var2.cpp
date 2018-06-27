#include <QCoreApplication>

#include <QSemaphore>

/** 
 * HEIG-TIC-PCO TE2 2015 Ex.1 (c) YTA Yann Thoma
 * Implementation: as a Monitor, with QWaitCondition and QMutex.
 * Version: student proposition, NOT verified, to avoid:
 *	- if maxTrams == 1, does the correct thing (no deadlock, and current direction liberated)
 *  - wake of next tram when accessing only if necessary
 *  - no more while needed.
 * WITH SEMAPHORE / NOT VERIFIED
 */
class Troncon {
protected:
	QSemaphore mutex; // protects all critic variables
	QSemaphore attente[2]; // wait condition for each direction
	int nbWaiting[2];
	int maxTrams; // nb max of tram on the critic segment
	int sensCourant; // current direction
	int nbTrams; // currrent nb of trams on the segment
	
public:
	Troncon(int maxTrams) : maxTrams(maxTrams), nbTrams(0) { }
	
	void acces(int sens)
	{
		mutex.acquire();
		
		/* we can access the railway if
		 * - there is room for a tram AND
		 * - the direction is the same OR no more trams are running in the segment
		 */
		++nbWaiting[sens];
		if((nbTrams >= maxTrams) || ((nbTrams > 0) && sensCourant != sens)) {
			mutex.release();
			attente[sens].acquire(); // mutex reception
		}
		--nbWaiting[sens];
		
		nbTrams++;
		sensCourant = sens;
		
		// if there are still trams waiting and there is room for them.
		if (nbWaiting[sens] != 0 && nbTrams < maxTrams) {
			attente[sens].release(); // mutex transmission
		} else {
			mutex.release();
		}
	}
	
	void sortie(int sens) {
		mutex.acquire();
		nbTrams--;
		if (nbWaiting[sens] != 0) {
			// we always, eventually wake up another tram in the same direction
			// there is room because we just made it!
			attente[sens].release(); // mutex transmission
		} else if (nbTrams == 0 && nbWaiting[1-sens] != 0) {
			// if we are the last tram
			// we switch direction and we wake up another tram in the opposite direction
			sensCourant = 1 - sensCourant;
			attente[1 - sens].release(); // mutex transmission
		} else {
			mutex.release();
		}
	}
};