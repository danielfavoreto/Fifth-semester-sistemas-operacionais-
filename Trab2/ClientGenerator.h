#ifndef CLIENT_GENERATOR_H
#define CLIENT_GENERATOR_H

#include <pthread.h>
#include "Client.h"

class ClientGenerator {

    // the shop
    BarberShop *shop;

    // number of customers
    unsigned int maxClientsTG;

    // overriding run() method
    static void* run(void*);

    // I have a  personal pthread
    pthread_t tid;

    public:
    // constructor
    ClientGenerator(BarberShop*, unsigned int);

    // destructor
    ~ClientGenerator();

    // client generating limit
    unsigned int getMaxClientsTG();

    // the thread start method
    void start();

    // joint the ClientGenerator object thread
    void joinClientGeneratorThread();

};

#endif