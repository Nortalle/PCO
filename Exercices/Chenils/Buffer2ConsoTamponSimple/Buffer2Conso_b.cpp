/** PCO 2015 YTA Buffer2Conso. Tampon simple, 1 producteur - 2 consommateurs.
 * --- 2eme solution --- Objectif: parfaitement fonctionnel.
 * Commentaire: optimisation ok, beaucoup mieux, générique (évidemment, plus compliqué). */
template<typename T> class Buffer2Conso : public AbstractBuffer<T> {
public:
	Buffer2Conso();
	virtual ~Buffer2Conso();
	virtual void put(T item);
	virtual T get(void);
private:
	T element;
	QSemaphore* waitProducteur;
	QSemaphore* waitConsommateur;
	QSemaphore* mutex; // protège les variables ci-dessous
	bool full; // indique l'état du tampon: plein ou vide.
	int nbWaitProducteur; // nombre de producteurs en attente
	int nbWaitConsommateur; // nombre de consommateurs en attente.
	int nbConsommation; // nombre de consommation effective du tampon.
};

Buffer2Conso::Buffer2Conso() {
	waitProducteur = new QSemaphore();
	waitConsommateur = new QSemaphore();
	mutex = new QSemaphore();
	mutex->release(); // libre
	full = false; // vide
}

Buffer2Conso::~Buffer2Conso() {
	delete waitProducteur;
	delete waitConsommateur;
	delete mutex;
}

void Buffer2Conso::put(T item) {
	mutex->acquire();
	if (full) { // prélude: si c'est déjà plein: on se met en attente.
		nbWaitProducteur++;
		mutex->release(); // liberation du mutex.
		waitProducteur->acquire();
		// pas besoin de mutex.acquire: transmission de mutex.
	}
	// critique: poser l'element.
	element = item;
	// post-lude: on libère un consommateur s'il y en a en attente
	if (nbWaitConsommateur > 0) {
		nbWaitConsommateur--;
		waitConsommateur->release(); // transmission de mutex
	} else {
		full = true;
		mutex->release(); // sinon on libère la ressource
	}
}

T Buffer2Conso::get(void) {
	T item;
	mutex->acquire();
	if(!full) { // prélude: s'il n'y a rien à consommer: on se met en attente.
		nbWaitConsommateur++;
		mutex->release();
		waitConsommateur->acquire();
		// pas besoin de mutex->acquire: transmission de mutex.
	}
	// section critique: récupérer l'élément.
	item = element;
	nbConsommation++; // il y a eu consommation!
	// post-lude: 1ere version (pas très claire)
	if (nbConsommation == 2 && nbWaitProducteur > 0) {
		nbConsommation = 0;
		nbWaitProducteur--;
		waitProducteur->release();  // transmission de mutex
	} else if (nbConsommation < 2) {
		if (nbWaitConsommateur > 0) {
			nbWaitConsommateur--;
			waitConsommateur->release();  // transmission de mutex
		} else {
			mutex->release();
		}
	} else {
		nbConsommation = 0;
		full = false;
		mutex->release();
	}
	// post-lude: plus simple et plus clair
	if (nbConsommation == 2) {
		nbConsommation = 0;
		// il y a eu 2 consommations: on peut libérer un producteur
		// s'il n'y en a pas, on libère la ressource et marque le tampon vide.
		if (nbWaitProducteur > 0) {
			nbWaitProducteur--;
			waitProducteur->release();  // transmission de mutex
		} else {
			full = false;
			mutex->release();
		}
	} else {
		// pas encore 2 consommations: on libère un autre consommateur en attente
		if (nbWaitConsommateur > 0) {
			nbWaitConsommateur--;
			waitConsommateur->release();  // transmission de mutex
		} else {
			mutex->release(); // sinon, on libère la ressource.
		}
	}
	// on retourne l'élément
	return item;
}