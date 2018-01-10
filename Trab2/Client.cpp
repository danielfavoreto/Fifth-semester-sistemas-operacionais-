#include <iostream>
#include "Client.h"
// default constructor
Client::Client(BarberShop *bs, unsigned int id): myid(id), shop(bs), haired(true) {
    // initate mutex and cond
    pthread_mutex_init(&hairMutex, nullptr);
    pthread_cond_init(&hairCondition, nullptr);
}

// default destructor
Client::~Client() {
    // avoid errors
    // cyclic pointers:
    // shop->clients->nodes points to Clients that points to shop
    shop = nullptr;
}

// compare clients by id
bool Client::operator==(const Client& c) {
    return myid == c.myid;
}

// get Thread id
pthread_t Client::getThreadId() {
    return tid;
}

// get My id
unsigned int Client::getId() {
    return myid;
}

// get hair status
bool Client::getHairStatus() {
    bool status;
    pthread_mutex_lock(&hairMutex);
    status = haired;
    pthread_mutex_unlock(&hairMutex);
    return status;
}

// get hair mutex
pthread_mutex_t* Client::getHairMutex() {
    return &hairMutex;
}

// get hair cond
pthread_cond_t* Client::getHairCondition() {
    return &hairCondition;
}

// running the client object
// private method
void* Client::run(void *arg) {
    // casting to Client pointer
    Client *me = (Client*) arg;
    if (nullptr == me) {
        std::cout << "Error! Invalid arg pointer to the client Thread!" << std::endl;
        return nullptr;
    }


    // add the current object to the BarberShop client list
    // sync method - if true, this client will take a chair and wait for the hair cut
    // the shop->addThreadClient() function notify the barbers about a new client
    if(me->shop->addThreadClient(me)) {

        // wait until shave or a nice hair cut
        while (me->getHairStatus()) {
            // lock the haired attribute
            pthread_mutex_lock(me->getHairMutex());
            // wait until hair cut
            std::cout << std::endl << "I'm the client " << me->getId() << ". I'm entering the BarberShop and I'm waiting for a hair cut!" << std::endl;
            pthread_cond_wait(me->getHairCondition(), me->getHairMutex());
            // unlock the haired attribute
            pthread_mutex_unlock(me->getHairMutex());
        }
        std::cout << std::endl << "I'm the client " << me->getId() << "! Got a nice hair cut!" << std::endl;
    } else {
        std::cout << std::endl << "Crowded here!! I'm client " << me->getId() << ". Let's try another BarberShop!" << std::endl;
        // lost client
        me->shop->addThreadLostClient(me);
    }

    pthread_exit(nullptr);
}
// the main object method
void Client::start() {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, run, this);
    return;
}

void Client::joinClientThread() {
    pthread_join(tid, nullptr);
}

// cut hair
void Client::cutHair() {
    pthread_mutex_lock(&hairMutex);
    haired = false;
    pthread_cond_signal(&hairCondition);
    pthread_mutex_unlock(&hairMutex);
}