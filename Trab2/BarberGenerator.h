#ifndef BARBER_GENERATOR_H
#define BARBER_GENERATOR_H

#include <pthread.h>
#include "Barber.h"

class BarberGenerator {

    // the shop
    BarberShop *shop;

    // number of customers
    unsigned int maxBarbersTG;

    // overriding run() method
    static void* run(void*);

    // I have a  personal pthread
    pthread_t tid;

    public:
        // constructor
        BarberGenerator(BarberShop*, unsigned int);

        // destructor
        ~BarberGenerator();

        // client generating limit
        unsigned int getMaxBarbersTG();

        // the thread start method
        void start();

        // joint the BarberGenerator object thread
        void joinBarberGeneratorThread();

};

#endif