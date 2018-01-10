#ifndef BARBER_SHOP_H
#define BARBER_SHOP_H

#include <pthread.h>

#include "SimpleList.h"

// forward declaring the required classes
class Client;
class Barber;

class BarberShop {

    // Barbers list
    List<Barber> *barbers;
    // maxBarbers
    unsigned int maxBarbers;
    // barber list mutex
    pthread_mutex_t barbersMutex;
    // barber list condition variable
    pthread_cond_t barbersCondition;

    // Client list or just the chairs
    List<Client> *clients;
    // max clients
    unsigned int maxClients;
    // client list mutex
    pthread_mutex_t clientsMutex;
    // client list condition variable
    pthread_cond_t clientsCondition;


    // is open?
    bool open;
    // BarberShop status mutex
    pthread_mutex_t openMutex;

    // lost clients 
    List<Client> *lost;
    // lost clients list mutex
    pthread_mutex_t lostMutex;
    // lost clients list condition variable
    pthread_cond_t lostCondition;

    public:
        // constructor
        // barbers list size and clients list size
        BarberShop(unsigned int, unsigned int);

        // destructor
        virtual ~BarberShop();

        // get client list mutex
        pthread_mutex_t* getClientsMutex();
        // get client list condition variable
        pthread_cond_t* getClientsCondition();

        // get barbers list mutex
        pthread_mutex_t* getBarbersMutex();
        // get barber list condition variable
        pthread_cond_t* getBarbersCondition();

        // get the maxBarbers value
        unsigned int getMaxBarbers();

        // get the maxClients value
        unsigned int getMaxClients();

        // add client to the list
        bool addThreadClient(Client*);

        // add client to the list
        void addThreadLostClient(Client *c);

        // get the first Client and
        // remove it from the linked list
        Client* getThreadClient();

        // add barber to the list
        bool addThreadBarber(Barber*);

        // remove client from the list
        void removeClient(Client *);

        // return the open attribute
        bool isOpen();

        // verify if there's clients waiting for a hair cut
        bool isThereClients();

        // close the shop
        void close();
        
        void joinLostClientsAndBarbers();
};

#endif