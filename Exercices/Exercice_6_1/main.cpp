#include <QCoreApplication>
#include <QThread>
#include <iostream>

using namespace std;
QThread *threads[8];

class T1 : public QThread{
public:
    void run() Q_DECL_OVERRIDE {
        cout << "Tache n°1" << endl;
    }
};

class T2 : public QThread{
public:
    void run() Q_DECL_OVERRIDE {
        threads[0]->wait();
        cout << "Tache n°2" << endl;
    }
};

class T3 : public QThread{
public:
    void run() Q_DECL_OVERRIDE {
        threads[0]->wait();
        cout << "Tache n°3" << endl;
    }
};

class T4 : public QThread{
public:
    void run() Q_DECL_OVERRIDE {

        threads[1]->wait();
        cout << "Tache n°4" << endl;
    }
};

class T5 : public QThread{
public:
    void run() Q_DECL_OVERRIDE {

        threads[1]->wait();
        cout << "Tache n°5" << endl;
    }
};

class T6 : public QThread{
public:
    void run() Q_DECL_OVERRIDE {
        threads[1]->wait();
        cout << "Tache n°6" << endl;
    }
};

class T7 : public QThread{
public:
    void run() Q_DECL_OVERRIDE {
        threads[3]->wait();
        threads[4]->wait();
        threads[5]->wait();
        cout << "Tache n°7" << endl;
    }
};

class T8 : public QThread{
public:
    void run() Q_DECL_OVERRIDE {
        threads[6]->wait();
        threads[2]->wait();
        cout << "Tache n°8" << endl;
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

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
