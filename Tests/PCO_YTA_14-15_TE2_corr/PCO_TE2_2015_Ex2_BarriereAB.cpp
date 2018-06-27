#include <QCoreApplication>

#include <QMutex>
#include <QWaitCondition>
/** 
 * HEIG-TIC-PCO TE2 2015 Ex.2 (c) YTA Yann Thoma
 * Implementation: as a Monitor, with QWaitCondition and QMutex.
 * Constraint: no for, no wakeAll.
 * Version: acknowledge in class.
 */
class BarriereAB {
protected:
	QMutex mutex;
	QWaitCondition cond[2];
	int nb[2];
	int size;
	
public:
	BarriereAB(int size) : size(size) {
		nb[0] = 0;
		nb[1] = 0;
	}
	
	void arrive(int classe) {
		mutex.lock();
		++nb[classe];
		// we block until we reach the size
		if(nb[classe] + nb[1 - classe] < size) {
			cond[classe].wait(&mutex);
		}
		// we leave
		--nb[classe];
		// if there are still one of my class: liberate one
		if(nb[classe] > 0) {
			cond[classe].wakeOne();
		} else if(nb[1 - classe] > 0) {
			// if there are still of the other class: liberate one
			cond[1 - classe].wakeOne();
		}
		mutex.unlock();
	}
};