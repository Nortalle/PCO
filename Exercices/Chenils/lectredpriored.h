#ifndef LECTREDPRIORED_H
#define LECTREDPRIORED_H


#include <QMutex>
#include <QSemaphore>

class LectRedPrioRed
{
private:
    QSemaphore mutex;
    int nbLecteurs;
    int nbRedacteurs;
    bool ecritureEnCours;
    QSemaphore attenteLecteurs;
    QSemaphore attenteRedacteur;
    QSemaphore nbRedacteursEnAttente;
    QSemaphore nbLecteursEnAttente;
public:
    LectRedPrioRed(){
        mutex.release();
        nbLecteurs = 0;
        nbRedacteurs = 0;
        nbRedacteursEnAttente = 0;
        ecritureEnCours = false;
    }
    void debutLecture(){
        mutex.acquire();
        if(ecritureEnCours){
            nbLecteursEnAttente ++;
            mutex.release();
            attenteLecteurs.acquire();
        }
        nbLecteurs ++;
        if(nbLecteursEnAttente > 0){
            nbLecteursEnAttente --;
            attenteLecteurs.release();
        }else{
            mutex.release();
        }
    }

    void debutEcriture(){
        mutex.acquire();
        if(nbLecteurs > 0 || ecritureEnCours){
            nbRedacteursEnAttente ++;
            mutex.release();
            attenteRedacteur.acquire();
        }
        ecritureEnCours = true;
        mutex.release();
    }

    void finLecture(){
        mutex.acquire();
        nbLecteurs --;
        if(nbRedacteursEnAttente > 0){
            nbRedacteursEnAttente--;
            attenteRedacteur.release();
        }else{
            mutex.release();
        }

    }

    void finEcriture(){
        mutex.acquire();
        ecritureEnCours = false;
        if(nbRedacteursEnAttente > 0){
            nbRedacteursEnAttente --;
            attenteRedacteur.release();
        }else if(nbLecteursEnAttente > 0){
            nbLecteursEnAttente --;
            attenteLecteurs.release();
        }else{
            mutex.release();
        }
    }

    ~LectRedPrioRed();
};

#endif // LECTREDPRIORED_H
