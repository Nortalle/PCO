
#include <QThread>
#include <QSemaphore>
#include <iostream>

QThread* threads[6];
QSemaphore* semaphores[4];

class T1 : public QThread
{
    void run() Q_DECL_OVERRIDE
    {

        std::cout << "Task 1" << std::endl;
        semaphores[0]->release();
        semaphores[0]->release();
    }
};

class T2 : public QThread
{
    void run() Q_DECL_OVERRIDE
    {

        semaphores[0]->acquire();
        std::cout << "Task 2" << std::endl;
        semaphores[1]->release();

    }
};

class T3 : public QThread
{
    void run() Q_DECL_OVERRIDE
    {
        semaphores[0]->acquire();
        std::cout << "Task 3" << std::endl;
        semaphores[2]->release();
    }
};


class T4 : public QThread
{
    void run() Q_DECL_OVERRIDE
    {

        semaphores[1]->acquire();
        std::cout << "Task 4" << std::endl;
        semaphores[3]->release();

    }
};

class T5 : public QThread
{
    void run() Q_DECL_OVERRIDE
    {
        semaphores[2]->acquire();
        std::cout << "Task 5" << std::endl;
        semaphores[3]->release();
    }
};

class T6 : public QThread
{
    void run() Q_DECL_OVERRIDE
    {
        semaphores[3]->acquire();
        semaphores[3]->acquire();
        std::cout << "Task 6" << std::endl;

    }
};


int main(int ,char *[])
{
    threads[0] = new T1();
    threads[1] = new T2();
    threads[2] = new T3();
    threads[3] = new T4();
    threads[4] = new T5();
    threads[5] = new T6();

    for(int i=0; i<4; i++){
        semaphores[i] = new QSemaphore(0);
    }

    for(int i=0; i< 6; i++)
        threads[i]->start();

    threads[5]->wait();
}
