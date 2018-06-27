#ifndef BRIDGE_TE1_MONITOR
#define BRIDGE_TE1_MONITOR

#define CAR_WEIGHT 1 // poids d'une voiture / car weight
#define TRUCK_WEIGHT 10 // poids d'un camion / truck weight 
#define MAX_WEIGHT 100 // poids supporte par le pont / max load on ONE bridge

#include <QWaitCondition>
#include <QMutex>

/** HEIG-TIC-PCO TE1 2015 Ex.4 (c) YTA Yann Thoma
 * Problem: Double Bridge with two waiting rows
 * Implementation: as a Monitor with QWaitCondition/QMutex only
 * WARNING: this version was NOT reviewed in class
 * Author: Valentin Minder */
class DoubleBridgeMonitor : public DoubleBridge {
private:
    QWaitCondition* waitingForTrack1; // blocage/file d'attente pour la voie1 
    QWaitCondition* waitingForTrack2; // blocage/file d'attente pour la voie2
    QMutex* mutex; // protèges les variables ci-dessous / protects all variables
    int weightOnTrack1; // poids courant sur la voie 1 / current weight track 1
    int weightOnTrack2; // poids courant sur la voie 2 / current weight track 2
    int nbWaitingForTrack2; // nombre de véhicule en attente sur la voie 2
public:
    DoubleBridgeMonitor() {
        waitingForTrack1 = new QWaitCondition();
        waitingForTrack2 = new QWaitCondition();
        mutex = new QMutex();
        mutex->unlock(); // ouvert/open
        weightOnTrack1 = weightOnTrack2 = nbWaitingForTrack2 = 0; // initialisation à 0.
    }
    virtual ~DoubleBridgeMonitor() {
        delete waitingForTrack1;
        delete waitingForTrack2;
        delete mutex;
    }

    virtual int carAccess() {
        mutex->lock();
        // essai sur la voie 1 / trial on track 1
        if(weightOnTrack1 + CAR_WEIGHT <= MAX_WEIGHT) {
            weightOnTrack1 += CAR_WEIGHT;
            mutex->unlock();
            return 1;
        }
        // trial on track 2 (only if there is NO truck waiting already)
        // essai sur la voie 2 (à condition qu'aucun camion ne soit en attente)
        if((weightOnTrack2 + CAR_WEIGHT <= MAX_WEIGHT) && (nbWaitingForTrack2 == 0)) {
            weightOnTrack2 += CAR_WEIGHT;
            mutex->unlock();
            return 2;
        }
        // otherwise, we wait on track 1 (and it will be track 1 anyway, even if track 2 is empty afterwards)
        // attente sur la voie 1 (la voie 1 sera empruntée, même si la 2 se libère entre temps!)
        // WHILE MANDATORY - always check with Monitors !
        while(weightOnTrack1 + CAR_WEIGHT > MAX_WEIGHT) {
             waitingForTrack1->wait(&mutex)
        }
        // no mutex->lock() because the wake of QWaitCondition already does it.
        // pas mutex->lock() car le wake de la condition le fait déjà.
        weightOnTrack1 += CAR_WEIGHT;
        mutex->release();
        return 1;
    }

    virtual void truckAccess(){
        mutex->lock();
        nbWaitingForTrack2++;
        // WHILE MANDATORY - always check with Monitors !
        while(weightOnTrack2 + TRUCK_WEIGHT > MAX_WEIGHT) {
            // no mutex->unlock() because the wait of QWaitCondition already does it.
            // pas mutex->unlock() car le wait de la condition le fait déjà.
            waitingForTrack2->wait(&mutex);
            // no mutex->lock() because the wake of QWaitCondition already does it.
            // pas mutex->lock() car le wake de la condition le fait déjà.
        }
        nbWaitingForTrack2--;
        weightOnTrack2 += TRUCK_WEIGHT;
        mutex->unlock();
    }

    virtual void carLeave(int voie){
        mutex->lock();

        if(voie == 1) {
            weightOnTrack1 -= CAR_WEIGHT;
            // liberation éventuelle d'autres sur la voie 1 (pas de vérification: le while du moniteur le fait)
            // we liberate another car on track 1 (no check needed: while in monitor does it)
            waitingForTrack1->wakeOne();
        } else {
            weightOnTrack2 -= CAR_WEIGHT;
            // liberation éventuelle d'autres sur la voie 2 (pas de vérification: le while du moniteur le fait)
            // we liberate another truck on track 2 (no check needed: while in monitor does it)
            waitingForTrack2->wakeOne();
        }
        mutex->unlock(); // to unlock in all cases (but: afterwards the wake!)
    }

    virtual void truckLeave(){
        mutex->lock();
        weightOnTrack2 -= TRUCK_WEIGHT;
        // liberation éventuelle d'autres sur la voie 2 (pas de vérification: le while du moniteur le fait)
        // we liberate another truck on track 2 (no check needed: while in monitor does it)
        waitingForTrack2->wakeOne();
        mutex->unlock(); // to unlock in all cases (but: afterwards the wake!)
    }
}; // class DoubleBridgeMonitor
#endif // BRIDGE_TE1_MONITOR