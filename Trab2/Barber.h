#ifndef BARBER_H
#define BARBER_H

#include "BarberShop.h"

class Barber {

    // the barber unique id
    unsigned int myid;

    // the shop pointer
    BarberShop *shop;

    // the thread method
    static void* run(void *);

    // I have a personal pthread
    pthread_t tid;

    public:
        // default constructor
        Barber(BarberShop*, unsigned int);
        // default destructor
        ~Barber();

        // get Thread id
        pthread_t getThreadId();

        // return the Barber id
        unsigned int getId();

        // the barber thread
        void start();

        // join the barber thread
        void joinBarberThread();
};

#endif