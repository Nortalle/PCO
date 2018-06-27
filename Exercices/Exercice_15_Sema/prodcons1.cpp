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

#include "buffer2conso.h"

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
  ITEM item = 0;
  while (true) {
    // produire item
    item ++;
    cout << "Tache producteur dépose" << endl;
    buffer->put(item);
    cout << "Tache producteur a déposé" << endl;
    usleep((int)((float)6000000*rand()/(RAND_MAX+1.0)));
  }
}

void Consumer::run() {
  ITEM item;
  while (true) {
    cout << "                                  Tache consommateur attend" << endl;
    item = buffer->get();
    cout << "                                  Tache consommateur a récupéré" << endl;
    // consommer item
    cout << "Item : " << item << endl;
    usleep((int)((float)6000000*rand()/(RAND_MAX+1.0)));
  }
}

int main(void) {
  Producer producers[NUM_THREADS_PRODUCER];
  Consumer consumers[NUM_THREADS_CONSUMER];

  //buffer = new Buffer2Conso<ITEM>();

  //buffer = new Buffer2ConsoSemaphore<ITEM>();
  buffer = new Buffer2Conso<ITEM>(4);
  //buffer = new Buffer2ConsoMesa<ITEM>();
  //buffer = new Buffer2ConsoHoare<ITEM>();

  for(int i=0; i<NUM_THREADS_PRODUCER; i++)
      producers[i].start();
  for(int i=0; i<NUM_THREADS_CONSUMER; i++)
      consumers[i].start();
  for(int i=0; i<NUM_THREADS_PRODUCER; i++)
    producers[i].wait();
  for(int i=0; i<NUM_THREADS_CONSUMER; i++)
    consumers[i].wait();
  return 0;
}
