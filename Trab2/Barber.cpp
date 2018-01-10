#include <iostream>
#include <unistd.h>
#include "Barber.h"
#include "Client.h"

// default constructor
Barber::Barber(BarberShop *bs, unsigned int id): myid(id), shop(bs) {}

// default destructor
Barber::~Barber() {
    shop = nullptr;
}

// get Thread id
pthread_t Barber::getThreadId() {
    return tid;
}

// return the Barber id
unsigned int Barber::getId() {
    return myid;
}

// the barber thread
void Barber::start() {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, run, this);
    return;
}

// join the barber thread
void Barber::joinBarberThread() {
    pthread_join(tid, nullptr);
}

void* Barber::run(void *arg) {
    // casting to Barber pointer
    Barber *me = (Barber*) arg;
    if (nullptr == me) {
        std::cout << "Error! Invalid arg pointer to the Barber Thread!" << std::endl;
        return nullptr;
    }
    std::cout << std::endl << "Barber " << me->getId() << " starting to shaving everybody!" << std::endl;


    // try to work at the shop
    if(me->shop->addThreadBarber(me)) {

        Client *client;

        pthread_mutex_t *clientsMutex = nullptr;
        pthread_cond_t *clientsCond = nullptr;

        while(me->shop->isOpen() || me->shop->isThereClients()) {

            client = me->shop->getThreadClient();
            if(nullptr == client && me->shop->isOpen()) {
                std::cout << std::endl << "Barber " << me->getId() << " says: nobody here! Let's take a nap!" << std::endl;
                clientsMutex = me->shop->getClientsMutex();
                clientsCond = me->shop->getClientsCondition();

                pthread_mutex_lock(clientsMutex);
                pthread_cond_wait(clientsCond, clientsMutex);
                pthread_mutex_unlock(clientsMutex);
            }

            if (nullptr != client) {
                sleep(1);
                client->cutHair();
                std::cout << std::endl << "Barber " << me->getId() << "! Done with client " << client->getId() << "!" << std::endl;
                // it's a gentleman
                client->joinClientThread();
//                 pthread_join(client->getThreadId(), nullptr);

                // remove?
                delete client;
            }
        }
        std::cout << std::endl << "Barber " << me->getId() << " says: good job for me!" << std::endl;

    } else {
        std::cout << std::endl << "Going home, there's no work to be done today!" << std::endl;
    }

    pthread_exit(nullptr);
}
