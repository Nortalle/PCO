#include <QCoreApplication>
#include <QThread>
#include <QSemaphore>
#include <iostream>

using namespace std;
QThread *threads[8];
QSemaphore *semaphores[4];


class T1 : public QThread{
public:
    void run() Q_DECL_OVERRIDE {
        cout << "Tache n°1" << endl;
        semaphores[0]->release();
        semaphores[0]->release();
    }
};

class T2 : public QThread{
public:
    void run() Q_DECL_OVERRIDE {
        semaphores[0]->acquire();
        cout << "Tache n°2" << endl;
        semaphores[1]->release();
        semaphores[1]->release();
        semaphores[1]->release();
    }
};

class T3 : public QThread{
public:
    void run() Q_DECL_OVERRIDE {
        semaphores[0]->acquire();
        cout << "Tache n°3" << endl;
        semaphores[3]->release();
    }
};

class T4 : public QThread{
public:
    void run() Q_DECL_OVERRIDE {
        semaphores[1]->acquire();
        cout << "Tache n°4" << endl;
        semaphores[2]->release();
    }
};

class T5 : public QThread{
public:
    void run() Q_DECL_OVERRIDE {
        semaphores[1]->acquire();
        cout << "Tache n°5" << endl;
        semaphores[2]->release();
    }
};

class T6 : public QThread{
public:
    void run() Q_DECL_OVERRIDE {
        semaphores[1]->acquire();
        cout << "Tache n°6" << endl;
        semaphores[2]->release();
    }
};

class T7 : public QThread{
public:
    void run() Q_DECL_OVERRIDE {
        semaphores[2]->acquire();
        semaphores[2]->acquire();
        semaphores[2]->acquire();
        cout << "Tache n°7" << endl;
        semaphores[3]->release();
    }
};

class T8 : public QThread{
public:
    void run() Q_DECL_OVERRIDE {
        semaphores[3]->acquire();
        semaphores[3]->acquire();
        cout << "Tache n°8" << endl;
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    for(size_t i = 0; i < 4; ++i){
        semaphores[i] = new QSemaphore(0);
    }

    threads[0] = new T1();
    threads[1] = new T2();
    threads[2] = new T3();
    threads[3] = new T4();
    threads[4] = new T5();
    threads[5] = new T6();
    threads[6] = new T7();
    threads[7] = new T8();

    for(QThread *cur : threads){
        cur->start();
    }

    return a.exec();
}
