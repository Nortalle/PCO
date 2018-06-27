

/******************************************************************************
Fichier: main.cpp
Auteur:  Yann Thoma
Date:    04.04.17.
Description: Gestion d'un pont pouvant supporter un certain poids.
    Les voitures pèsent 1, et les camions 10. L'accès au pont se fait sans
    dépassement, et le protocole doit assurer que la charge maximale n'est
    jamais dépassée. Ce fichier implémente un checker et le lancement des
    threads.
******************************************************************************/

#include <iostream>

#include <QThread>
#include <QMutex>

#include "bridgemanager.h"

#define NUM_THREADS_VOITURE  50
#define NUM_THREADS_CAMION   10
#define POIDS_MAX           100


class BridgeChecker
{
protected:
    int nbCars;
    int nbTrucks;
    QMutex mutex;


    void check()
    {
        if (totalWeight() > POIDS_MAX) {
            std::cerr << "Error. Too many vehicles on the bridge" << std::endl;
        }
    }


public:

    BridgeChecker() : nbCars(0), nbTrucks(0), mutex(QMutex::Recursive) {};

    int totalWeight() {
        int result;
        mutex.lock();
        result = nbCars + 10*nbTrucks;
        mutex.unlock();
        return result;
    }

    void addCar()
    {
        mutex.lock();
        nbCars ++;
        check();
        mutex.unlock();
    }

    void addTruck()
    {
        mutex.lock();
        nbTrucks ++;
        check();
        mutex.unlock();
    }

    void removeCar()
    {
        mutex.lock();
        nbCars --;
        check();
        mutex.unlock();
    }

    void removeTruck()
    {
        mutex.lock();
        nbTrucks --;
        check();
        mutex.unlock();
    }

};

BridgeChecker checker;



class CarTask : public QThread
{
    BridgeManager *manager;
    int tid;

public:
    CarTask(BridgeManager *manager,int tid) : manager(manager), tid(tid) {};

    virtual void run()
    {
        while(1) {
            manager->carAccess();
            checker.addCar();
            std::cout << "Tache " << tid << ": Voiture sur le pont" << std::endl;
            std::cout << "Poids: " << checker.totalWeight() << std::endl;
            usleep((int)((float)6000000*rand()/(RAND_MAX+1.0)));
            checker.removeCar();
            manager->carLeave();
            usleep((int)((float)1000000*rand()/(RAND_MAX+1.0)));
        }

    }
};

class TruckTask : public QThread
{
    BridgeManager *manager;
    int tid;

public:
    TruckTask(BridgeManager *manager,int tid) : manager(manager), tid(tid) {};

    virtual void run()
    {
        while(1) {
            manager->truckAccess();
            checker.addTruck();
            std::cout << "Tache " << tid << ": Camion sur le pont" << std::endl;
            std::cout << "Poids: " << checker.totalWeight() << std::endl;
            usleep((int)((float)6000000*rand()/(RAND_MAX+1.0)));
            checker.removeTruck();
            manager->truckLeave();
            usleep((int)((float)1000000*rand()/(RAND_MAX+1.0)));
        }
    }
};


void test(BridgeManager *manager,int nbCars, int nbTrucks)
{
    CarTask* threadsVoiture[nbCars];
    TruckTask* threadsCamion[nbTrucks];
    int t;
    for(t=0; t<nbCars; t++){
        std::cout << "Création de la tâche voiture " << t << std::endl;
        threadsVoiture[t] = new CarTask(manager,t);
        threadsVoiture[t]->start();
    }
    for(t=0; t<nbTrucks; t++){
        std::cout << "Création de la tâche camion " << t << std::endl;
        threadsCamion[t] = new TruckTask(manager,t);
        threadsCamion[t]->start();
    }

    for(t=0; t<nbCars; t++){
        threadsVoiture[t]->wait();
        delete threadsVoiture[t];
    }
    for(t=0; t<nbTrucks; t++){
        threadsCamion[t]->wait();
        delete threadsCamion[t];
    }

}

int main(int /*argc*/, char */*argv*/[])
{

    BridgeManager *manager;
    manager = new BridgeManager(POIDS_MAX);

    test(manager,NUM_THREADS_VOITURE,NUM_THREADS_CAMION);
}
