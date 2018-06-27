#ifndef BRIDGE_TE1_SEMAPHORE
#define BRIDGE_TE1_SEMAPHORE

#define CAR_WEIGHT 1 // poids d'une voiture / car weight
#define TRUCK_WEIGHT 10 // poids d'un camion / truck weight 
#define MAX_WEIGHT 100 // poids supporte par le pont / max load on ONE bridge

#include <QSempahore>

/** HEIG-TIC-PCO TE1 2015 Ex.4 (c) YTA Yann Thoma
 * Problem: Double Bridge with two waiting rows
 * Implementation: with QSemaphore only
 * This version was reviewed in class.
 * Author: Valentin Minder
 */
class DoubleBridgeSemaphore : public DoubleBridge{
private:
    QSemaphore *waitingForTrack1; // blocage/file d'attente pour la voie1 
    QSemaphore *waitingForTrack2; // blocage/file d'attente pour la voie2
    QSemaphore *mutex; // protèges les variables ci-dessous
    int weightOnTrack1; // poids courant sur la voie 1 / current weight track 1
    int weightOnTrack2; // poids courant sur la voie 2 / current weight track 2
    int nbWaitingForTrack1; // nombre de véhicule en attente sur la voie 1
    int nbWaitingForTrack2; // nombre de véhicule en attente sur la voie 2
public:
    DoubleBridgeSemaphore() {
        waitingForTrack1 = new QSemaphore(0); // fermé / closed
        waitingForTrack2 = new QSemaphore(0); // fermé / closed
        mutex = new QSemaphore(1); // ouvert / open
        weightOnTrack1 = weightOnTrack2 = nbWaitingForTrack1 = nbWaitingForTrack2 = 0; // initialisation à 0.
    }
    virtual ~DoubleBridgeSemaphore() {
        delete waitingForTrack1;
        delete waitingForTrack2;
        delete mutex;
    }

    virtual int carAccess() {
        mutex->acquire();
        // essai sur la voie 1 / trial on track 1
        if(weightOnTrack1 + CAR_WEIGHT <= MAX_WEIGHT) {
            weightOnTrack1 += CAR_WEIGHT;
            mutex->release();
            return 1;
        }
        // trial on track 2 (only if there is NO truck waiting already)
        // essai sur la voie 2 (à condition qu'aucun camion ne soit en attente)
        if((weightOnTrack2 + CAR_WEIGHT <= MAX_WEIGHT) && (nbWaitingForTrack2 == 0)) {
            weightOnTrack2 += CAR_WEIGHT;
            mutex->release();
            return 2;
        }
        // otherwise, we wait on track 1 (and it will be track 1 anyway, even if track 2 is empty afterwards)
        // attente sur la voie 1 (la voie 1 sera empruntée, même si la 2 se libère entre temps!)
        nbWaitingForTrack1++;
        mutex->release();
        waitingForTrack1->acquire();
        // no mutex->acquire() because of mutex transmission (reception)
        // pas mutex->acquire() car transmission de mutex (réception)
        weightOnTrack1 += CAR_WEIGHT;
        mutex->release();
        return 1;
    }

    virtual void truckAccess(){
        mutex->acquire();
        if(weightOnTrack2 + TRUCK_WEIGHT >= MAX_WEIGHT) {
            nbWaitingForTrack2++;
            mutex->release();
            waitingForTrack2->acquire();
            // no mutex->acquire() because of mutex transmission (reception)
            // pas mutex->acquire() car transmission de mutex (réception)
        }
        weightOnTrack2 += TRUCK_WEIGHT;
        mutex->release();
    }

    virtual void carLeave(int voie){
        mutex->acquire();
        
        if(voie == 1) {
            weightOnTrack1 -= CAR_WEIGHT;
            // liberation éventuelle d'autres sur la voie 1
            // (la condition est ok puisqu'une voiture vient de partir, et on libère une voiture)
            // we liberate another car on track 1 (it's okay because a car just left)
            if(nbWaitingForTrack1 > 0) {
                nbWaitingForTrack1--;
                waitingForTrack1->release(); // transmission mutex (sent)
            } else {
                mutex->release();
            }
        } else {
            weightOnTrack2 -= CAR_WEIGHT;
            // liberation éventuelle d'autres sur la voie 2 (à condition que se soit possible)
            // (on vérifie la condition puisqu'une voiture vient de partir et on libère un camion
            // we liberate another truck on track 2 (we have to check because a car just left)
            if((nbWaitingForTrack2 > 0) && (weightOnTrack2 + TRUCK_WEIGHT) <= MAX_WEIGHT ) {
                nbWaitingForTrack2--;
                waitingForTrack2->release(); // transmission mutex (sent)
            } else {
                mutex->release();
            }
        }
    }

    virtual void truckLeave(){
        mutex->acquire();
        weightOnTrack2 -= TRUCK_WEIGHT;
        // liberation éventuelle d'autres sur la voie 2
        // (la condition est ok puisqu'un camion vient de partir, et on libère un camion)
        // we liberate another truck on track 2 (it's okay because a truck just left)
        if(nbWaitingForTrack2 > 0) {
            nbWaitingForTrack2--;
            waitingForTrack2->release(); // mutex transmission (sent)
        } else {
            mutex->release();
        }
    }
}; // class DoubleBridgeSemaphore
#endif // BRIDGE_TE1_SEMAPHORE