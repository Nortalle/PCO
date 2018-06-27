/**
 * PCO Exam 2015 YTA Ex. 3 Tampon simple à N consommations
 * Warning: proposition de correction sans garantie !
 */
#include <QMutex>
#include <QWaitCondition>

<template: Item>
Buffer1N {
private:
	Item buffer; // valeur du tampon 
	const int nbConso; // nb max de consommations
	int nbCurrent; // nb courant de consommations du tampon
	QMutex mutex;
	QWaitCondition waitConso; // met en attente les consommateurs
	QWaitCondition waitProducer; // met en attente les producteurs
public:
	Buffer1N(int nbConso) : nbConso(nbConso) {}

	void put (Item item) {
		mutex.lock();
		// attente que le tampon soit vide et remis à zéro
		while (buffer != NULL) {
			waitProducer.wait(&mutex);
		} 

		buffer = item;
		
		// réveil éventuel d'un consommateur
		// (si plusieurs consommateurs: s'appelent entre eux...)
		waitConso.wakeOne();

		mutex.unlock();
	}

	Item get() {
		mutex.lock();
		Item result;

		// attente que le tampon soit plein
		while (buffer == NULL) {
			waitConso.wait(&mutex);
		}

		result = buffer;
		++nbCurrent;

		if (nbCurrent == nbConso) {
			// reset et réveil eventuel d'un producteur
			nbCurrent = 0;
			buffer = NULL;
			waitProducer.wakeOne();
		} else {
			// réveil éventuel d'un autre consommateur.
			waitConso.wakeOne();
		}

		mutex.unlock();
		return result;
	}
}; // Buffer1N