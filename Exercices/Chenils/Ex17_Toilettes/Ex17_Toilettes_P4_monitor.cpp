/*
PCO Concurrent Programming / Ex. 17 - Toilettes
Valentin Minder / Eleonore d'Agostino / David Kunzmann / Paul Nta / Karim Ghozlani
POLITIQUE 4: after N people of same sex, we switch
Implementation: with monitors only.
*/

#include <QWaitCondition>
#include <QMutex>

class AbstractToilet {
public:
	AbstractToilet(int nbSeats) : nbSeats(nbSeats) {}; 
	virtual void manAccessing() = 0;
	virtual void manLeaving() = 0;
	virtual void womanAccessing() = 0;
	virtual void womanLeaving() = 0;
protected:
	// number of seats in the toilets 
	int nbSeats;
};

class ConcreteNMonToilet : public AbstractToilet {

/*
Let nbMaxPeopleInRow the number of people of same sex that can go in the same row.
Let currentNbPeopleInRow = 0 the number of people that went so far in the same row.
At each entry we do ncurrentNbPeopleInRow++. When switching sex we do currentNbPeopleInRow = 0.

Soit nbMaxPeopleInRow le nombre nombre de personnes du même sexe qui peuvent passer à la suite.
Soit currentNbPeopleInRow = 0 le nombre de personnes du même sexe qui sont déjà passées à la suite.
A chaque entrée, on fait currentNbPeopleInRow++. Lors de changement de sexes on fait currentNbPeopleInRow = 0.
*/
private: 
// N - max number of same sex before a switch
int nbMaxPeopleInRow = 30;
// current number of same sex in the same row
int currentNbPeopleInRow= 0;

// number of people in the toilets now
int nbManInside = 0;
int nbWomanInside = 0;

// Wait condition
QWaitCondition* womanWaiting, manWaiting;
// number of people waiting
int nbWomanWaiting = 0;
int nbManWaiting = 0;

// global mutex for all variables
QMutex* mutex;

public:
ConcreteNMonToilet(int nbSeats, int nbMaxPeopleInRow) : AbstractToilet(nbSeats), nbMaxPeopleInRow(nbMaxPeopleInRow) {
	womanWaiting = new QWaitCondition();
	manWaiting = new QWaitCondition();
	mutex = new QMutex();
}

~ConcreteNMonToilet() {
	delete womanWaiting;
	delete manWaiting;
	delete mutex;
}

/*
A woman can acces the toilet (currentNbPeopleInRow++) if:
- the number of man inside the toilets is 0
- AND there is still room in the toilets
- AND the number of women in the same row didn't reach the maximum 
Otherwise: blocks and wait.
=> wait until a woman goes out, or all men are gone.

Une femme peut accéder aux toilettes (currentNbPeopleInRow++) si:
- le nombre d'hommes dans les toilettes vaut 0
- ET il y a encore de la place dans les toilettes
- ET que le nombre de femmes à la suite n'a pas encore atteint son maximum
Sinon: bloque, se met en attente. 
=> attendra qu'une femme sorte, ou que tous les hommes soient sortis
*/
virtual void womanAccessing() {
	mutex->lock();
	nbWomanWaiting++;
	while (nbManInside != 0 || nbWomanInside >= nbSeats || currentNbPeopleInRow >= nbMaxPeopleInRow) {
		womanWaiting->wait(&mutex);
	}
	nbWomanWaiting--;
	nbWomanInside++;
	currentNbPeopleInRow++;
	mutex->unlock();
}

/*
A man can acces the toilet (currentNbPeopleInRow++) if:
- the number of woman inside the toilets is 0
- AND there is still room in the toilets
- AND the number of men in the same row didn't reach the maximum 
Otherwise: blocks and wait.
=> wait until a man goes out, or all women are gone.

Un homme peut accéder aux toilettes (currentNbPeopleInRow++) si
- le nombre de femmes dans les toilettes vaut 0
- ET il y a encore de la place dans les toilettes
- ET que le nombre d'hommes à la suite n'a pas encore atteint son maximum
Sinon: bloque, se met en attente. 
=> attendra qu'un homme sorte, OU alors que toutes les femmes sortent
*/
virtual void manAccessing() {
	mutex->lock();
	nbManWaiting++;
	while (nbWomanInside != 0 || nbManInside >= nbSeats || currentNbPeopleInRow >= nbMaxPeopleInRow) {
		manWaiting->wait(&mutex);
	}
	nbManWaiting--;
	nbManInside++;
	currentNbPeopleInRow++;
	mutex->unlock();
}

/*
When a woman comes out of the toilet:
- If there is a woman waiting, and we didn't reach the maximum of women in the same row, we liberate her (=> next woman)
- Otherwise, and if the toilets are free (the number of women inside = 0), we reset the number of people in the same row.
	Additionaly, if there are men waiting, we can liberate them (up to the number of seats in the toilets)
- Otherwise, we do nothing (=> there is no one waiting, OR men are waiting but there are still women in the toilets, OR too many women went in the same row and we stop them)

A la sortie d'une femme: 
- S'il y a une femme une attente, et que nb < N, on la libère. (=> passage à la femme suivante, nb++)
- Sinon, si les toilettes sont vides (aka le nombre de femmes dans les toilettes = 0), on remet à zero le nombre de personnes à la suite.
	De plus, s'il y a au moins un homme en attente, on les libère (jusqu'à max la capacité des toilettes). (=> c'est le tour des hommes, il peuvent y aller en groupe jusqu'à la taille des toilettes) nb = 0
- Sinon, on ne fait rien. (=> il n'y a plus personne en attente, OU alors des hommes en attente mais il y a encore des femmes dans les toilettes, ou alors currentNbPeopleInRowfemmes sont déjà passées et on stoppe les femmes)
*/

virtual void womanLeaving() {
	mutex->lock();
	nbWomanInside--;
	if (nbWomanWaiting > 0 && currentNbPeopleInRow < nbMaxPeopleInRow) {
		womanWaiting->wakeOne();
	} else if (nbWomanInside == 0) {
		currentNbPeopleInRow = 0;
		for (int i = 0; i < nbSeats; i++) {
			manWaiting->wakeOne();
		}
	}
	mutex->unlock();
}

/*
When a man comes out of the toilet:
- If there is a man waiting, and we didn't reach the maximum of men in the same row, we liberate him (=> next man)
- Otherwise, and if the toilets are free (the number of men inside = 0), we reset the number of people in the same row.
	Additionaly, if there are women waiting, we can liberate them (up to the number of seats in the toilets)
- Otherwise, we do nothing (=> there is no one waiting, OR women are waiting but there are still men in the toilets, OR too many men went in the same row and we stop them)

A la sortie d'un homme:
- S'il y a au moins un homme en attente, et que nb < N, on libère un homme. (=> passage à l'homme suivant)
- Sinon, si c'est le dernier homme à sortir (aka le nombre d'homme dans les toilettes = 0), on remet à zero le nombre de personnes à la suite.
	De plus, s'il y a au moins une femme en attente, on libère les femmes (jusqu'à max la capacité des toilettes) (=> c'est le tour des femmes, elles peuvent y aller en groupe jusqu'à la taille des toilettes) nb = 0
- Sinon, on ne fait rien. (=> il n'y a plus personne en attente, OU alors des femmes en attente mais il y a encore des hommes dans les toilettes, ou alors currentNbPeopleInRowhommes sont déjà passées et on stoppe les hommes)
*/

virtual void manLeaving() {
	mutex->lock();
	nbManInside--;
	if (nbManWaiting > 0 && currentNbPeopleInRow < nbMaxPeopleInRow) {
		manWaiting->wakeOne();
	} else if (nbManInside == 0) {
		currentNbPeopleInRow = 0;
		for (int i = 0; i < nbSeats; i++) {
			womanWaiting->wakeOne();
		}
	}
	mutex->unlock();
}

}; // fin de classe.