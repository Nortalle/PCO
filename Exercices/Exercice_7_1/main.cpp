
#include <QThread>
#include <iostream>

QThread* threads[6];

class T1 : public QThread
{
    void run() Q_DECL_OVERRIDE
    {

        std::cout << "Task 1" << std::endl;

    }
};

class T2 : public QThread
{
    void run() Q_DECL_OVERRIDE
    {
        threads[0]->wait();
        std::cout << "Task 2" << std::endl;

    }
};

class T3 : public QThread
{
    void run() Q_DECL_OVERRIDE
    {
        threads[0]->wait();
        std::cout << "Task 3" << std::endl;

    }
};


class T4 : public QThread
{
    void run() Q_DECL_OVERRIDE
    {
        threads[1]->wait();
        std::cout << "Task 4" << std::endl;

    }
};

class T5 : public QThread
{
    void run() Q_DECL_OVERRIDE
    {
        threads[2]->wait();
        std::cout << "Task 5" << std::endl;

    }
};

class T6 : public QThread
{
    void run() Q_DECL_OVERRIDE
    {
        threads[3]->wait();
        threads[4]->wait();
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

    for(int i=0; i< 6; i++)
        threads[i]->start();

    threads[5]->wait();
}
