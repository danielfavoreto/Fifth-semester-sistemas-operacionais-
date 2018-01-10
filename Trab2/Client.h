#ifndef CLIENT_H
#define CLIENT_H

#include <pthread.h>

#include "BarberShop.h"

class Client {

    // Client id
    unsigned int myid;

    // the shop pointer
    BarberShop *shop;

    // hair status
    bool haired;
    // hair mutex
    pthread_mutex_t hairMutex;
    // hair condition variable
    pthread_cond_t hairCondition;

    // @Override
    static void* run(void *);

    // I have a personal tread
    pthread_t tid;

    public:
        Client(BarberShop*, unsigned int);
        ~Client();

        // compare clients by id
        bool operator==(const Client&);

        // get Thread id
        pthread_t getThreadId();

        // get My id
        unsigned int getId();

        // get hair status
        bool getHairStatus();

        // get hair mutex
        pthread_mutex_t* getHairMutex();

        // get hair cond
        pthread_cond_t* getHairCondition();

        // the main object method
        void start();

        // join the client thread to exit
        void joinClientThread();

        // cut hair
        void cutHair();
};

#endif