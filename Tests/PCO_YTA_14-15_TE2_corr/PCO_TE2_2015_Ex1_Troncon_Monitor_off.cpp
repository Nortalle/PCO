#include <QCoreApplication>

#include <QMutex>
#include <QWaitCondition>

/** 
 * HEIG-TIC-PCO TE2 2015 Ex.1 (c) YTA Yann Thoma
 * Implementation: as a Monitor, with QWaitCondition and QMutex.
 * Version: acknowledge in class.
 */
class Troncon {
protected:
	QMutex mutex; // protects all critic variables
	QWaitCondition attente[2]; // wait condition for each direction
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
		while((nbTrams >= maxTrams) || ((nbTrams > 0) && sensCourant != sens)) {
			attente[sens].wait(&mutex);
		}
		
		nbTrams++;
		sensCourant = sens;
		attente[sens].wakeOne(); // we eventually wake up another tram (same direction)
		mutex.unlock();
	}
	
	void sortie(int sens) {
		mutex.lock();
		nbTrams--;
		if (nbTrams == 0) {
			// if we are the last tram
			// we switch direction and we wake up another tram in the opposite direction
			sensCourant = 1 - sensCourant;
			attente[1 - sens].wakeOne();
		} else {
			// otherwisw, we eventually wake up another tram in the same direction
			attente[sens].wakeOne();
		}
		mutex.unlock();
	}
};