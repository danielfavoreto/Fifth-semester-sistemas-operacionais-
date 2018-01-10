#include <iostream>
#include "BarberShop.h"
#include "Client.h"
#include "Barber.h"

// constructor
// barbers list size and 
BarberShop::BarberShop(unsigned int bSize, unsigned int cSize): maxBarbers(bSize), maxClients(cSize), open(true) {
    // instantiate a new template based list object
    clients = new List<Client>();

    lost = new List<Client>();

    // create the barbers list
    barbers = new List<Barber>();

    // clients mutex and cond var
    pthread_mutex_init(&clientsMutex, nullptr);
    pthread_cond_init(&clientsCondition, nullptr);

    // barbers mutex and cond var
    pthread_mutex_init(&barbersMutex, nullptr);
    pthread_cond_init(&barbersCondition, nullptr);

    // lost clients mutex and cond var
    pthread_mutex_init(&lostMutex, nullptr);
    pthread_cond_init(&lostCondition, nullptr);

    // open mutex
    pthread_mutex_init(&openMutex, nullptr);
}

// destructor
BarberShop::~BarberShop() {
    // see the List destructor
    delete clients;
    delete lost;
    delete barbers;
}

// get client list mutex
pthread_mutex_t* BarberShop::getClientsMutex() {
    return &clientsMutex;
}

// get client list condition variable
pthread_cond_t* BarberShop::getClientsCondition() {
    return &clientsCondition;
}

// get barbers list mutex
pthread_mutex_t* BarberShop::getBarbersMutex() {
    return &barbersMutex;
}
// get barber list condition variable
pthread_cond_t* BarberShop::getBarbersCondition() {
    return &barbersCondition;
}

// get the maxBarbers value
unsigned int BarberShop::getMaxBarbers(){
    return maxBarbers;
}

// get the maxClients value
unsigned int BarberShop::getMaxClients() {
    return maxClients;
}

// add client to the list
bool BarberShop::addThreadClient(Client *c) {
    if (nullptr != c) {
        bool available = false;
        // lock the clients list, or the chairs
        pthread_mutex_lock(&clientsMutex);
        // add the client to the list
        if (maxClients > clients->length()) {
            available = true;
            clients->append(c);
            // notify barber about me
            pthread_cond_signal(&clientsCondition);
        }
        // unlock the clients list, or the chairs
        pthread_mutex_unlock(&clientsMutex);
        // return info to the client, maybe there's no space
        return available;
    }
    return false;
}

// add client to the list
void BarberShop::addThreadLostClient(Client *c) {
    if (nullptr != c) {
        // lock the clients list
        pthread_mutex_lock(&lostMutex);
        // add the lost client to the list
        lost->append(c);
        // unlock the lost clients list
        pthread_mutex_unlock(&lostMutex);
        // return info to the client, maybe there's no space
    }
}

// get the first Client and 
// remove it from the linked list
Client* BarberShop::getThreadClient() {
    pthread_mutex_lock(&clientsMutex);
    Client *c = clients->poll();
    if (nullptr != c) {
        std::cout << std::endl << "Client " << c->getId() << " attended!" << std::endl;
    }
    pthread_mutex_unlock(&clientsMutex);
    return c;
}

// add barber to the list
bool BarberShop::addThreadBarber(Barber *b) {
    if (nullptr != b) {
        bool available = false;
        // lock the barbers list
        pthread_mutex_lock(&barbersMutex);
        // add the client to the list
        if (maxBarbers > barbers->length()) {
            available = true;
            barbers->append(b);
        }
        // unlock the barbers list, or the chairs
        pthread_mutex_unlock(&barbersMutex);
        // return info to the barber, maybe there's no job here
        
        return available;
    }
    return false;
}

// remove client from the list
void BarberShop::removeClient(Client *c) {
    // lock the clients list, or the chairs
    pthread_mutex_lock(&clientsMutex);
    // remove the client by id
    clients->remove(c);
    // unlock the clients list, or the chairs
    pthread_mutex_unlock(&clientsMutex);
}

// return the open attribute
bool BarberShop::isOpen() {
    return open;
}
// verify if there's clients waiting for a hair cut
bool BarberShop::isThereClients() {
    return 0 < clients->length();
}

void BarberShop::close() {
    // close the shop
    pthread_mutex_lock(&openMutex);
    open = false;
    pthread_mutex_unlock(&openMutex);

    // wake up barbers
    pthread_mutex_lock(&clientsMutex);
    pthread_cond_broadcast(&clientsCondition);
    pthread_mutex_unlock(&clientsMutex);
}

void BarberShop::joinLostClientsAndBarbers() {
    Barber * b = nullptr;
    while ((b = barbers->iterator()) != nullptr) {
        pthread_join(b->getThreadId(), nullptr);
    }

    Client *c = nullptr;
    while ((c = lost->poll()) != nullptr) {
        pthread_join(c->getThreadId(), nullptr);
        delete c;
    }
}