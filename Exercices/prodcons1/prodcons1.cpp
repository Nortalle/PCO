/******************************************************************************
Fichier: exemple.c
Auteur:  Yann Thoma
Date:    05.05.09.
Description: Tampon simple.
    Les producteur doivent attendre qu'un consommateur ait lu
    la donnée avant de pouvoir en remettre une nouvelle.
******************************************************************************/


#include <iostream>
#include <semaphore.h>

#include <QThread>
#include <QSemaphore>

#include "buffer1.h"
#include "buffern.h"

using namespace std;

#define NUM_THREADS_CONSUMER   4
#define NUM_THREADS_PRODUCER   2

#define ITEM int


static AbstractBuffer<ITEM> *buffer;

class Producer : public QThread
{
private:
    void run() Q_DECL_OVERRIDE;
};

class Consumer : public QThread
{
private:
    void run() Q_DECL_OVERRIDE;
};

void Producer::run() {
  ITEM item;
  while (true) {
    // produire item
    cout << "Tache producteur dépose" << endl;
    buffer->put(item);
    cout << "Tache producteur a déposé" << endl;
  }
}

void Consumer::run() {
  ITEM item;
  while (true) {
    cout << "                                  Tache consommateur attend" << endl;
    item = buffer->get();
    cout << "                                  Tache consommateur a récupéré" << endl;
    // consommer item
  }
}

int main(void) {
  Producer prod;
  Consumer cons;
  buffer = new Buffer1<ITEM>();
  prod.start();
  cons.start();
  prod.wait();
  cons.wait();
  return 0;
}

/*
void *tacheProducteur(void *arg) {
    int tid;
    tid = (int)(intptr_t)arg;
    while(1) {
        cout << "Tache producteur " << tid << ": dépose" << std::endl;
        tampon->put(tid);
        cout << "Tache producteur " << tid << ": a déposé" << std::endl;
        usleep((int)((float)6000000*rand()/(RAND_MAX+1.0)));
    }
    pthread_exit(NULL);
}

void *tacheConsommateur(void *arg) {
    int tid;
    tid = (int)(intptr_t)arg;
    while(1) {
        cout << "                                  Tache consommateur " << tid << ": attent" << endl;
        // On n'utilise pas la donnée, on ne fait que la prélever
        tampon->get();
        cout << "                                  Tache consommateur " << tid << ": a récupéré" << endl;
        usleep((int)((float)6000000*rand()/(RAND_MAX+1.0)));
    }
    pthread_exit(NULL);
}

int main (int argc, char **argv[])
{
    pthread_t threadsRed[NUM_THREADS_PRODUCER];
    pthread_t threadsLec[NUM_THREADS_CONSUMER];
    int rc, t;
    int test = 3;
    switch (test) {
    case 0: tampon = new Buffer1<ITEM>();break;
    case 1: tampon = new Buffer1a<ITEM>();break;
    case 2: tampon = new BufferN<ITEM>(4);break;
    case 3: tampon = new BufferNa<ITEM>(4);break;
    }

    for(t=0; t<NUM_THREADS_CONSUMER; t++){
        cout << "Création de la tâche " << t << endl;
        rc = pthread_create(&threadsLec[t], NULL, tacheConsommateur, (void *)(intptr_t)t);
        if (rc){
            cerr << "ERREUR: le code de retour de pthread_create() est " << rc << endl;
            exit(-1);
        }
    }
    for(t=0; t<NUM_THREADS_PRODUCER; t++){
        cout << "Création de la tâche " << t << endl;
        rc = pthread_create(&threadsRed[t], NULL, tacheProducteur, (void *)(intptr_t)t);
        if (rc){
            cerr << "ERREUR: le code de retour de pthread_create() est " << rc << endl;
            exit(-1);
        }
    }

    for(t=0; t<NUM_THREADS_CONSUMER; t++){
        pthread_join(threadsLec[t],NULL);
    }
    for(t=0; t<NUM_THREADS_PRODUCER; t++){
        pthread_join(threadsRed[t],NULL);
    }
    pthread_exit(NULL);
}
*/
