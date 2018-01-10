#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "ClientGenerator.h"

// constructor
ClientGenerator::ClientGenerator(BarberShop *bs, unsigned int max): shop(bs), maxClientsTG(max) {}

// destructor
ClientGenerator::~ClientGenerator() {
    shop = nullptr;
}

// client generating limit
unsigned int ClientGenerator::getMaxClientsTG() {
    return maxClientsTG;
}

// the thread start method
void ClientGenerator::start() {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, run, this);
    return;
}

// joint the ClientGenerator
void ClientGenerator::joinClientGeneratorThread() {
    pthread_join(tid, nullptr);
}

// overriding Runnable run() method
void* ClientGenerator::run(void *arg) {

    ClientGenerator *cg = (ClientGenerator*) arg;
    if (nullptr == cg) {
        std::cout << "Error! Invalid ClientGenerator pointer";
        pthread_exit(nullptr);
    }

    std::cout << std::endl << "---- Haired clients comming!! ----" << std::endl;

    unsigned int i = 0;

    Client *c = nullptr;
    // see maxClients
    while(i < cg->getMaxClientsTG()) {

        // create a Client Thread
        c = new Client(cg->shop, i+1);

        c->start();

        // try to sleep 700 ms
        usleep(rand()%400000 + 1);
        // see maxClients
        i++;
    }

    c = nullptr;
    std::cout << "Done! We are closing now! But all remaining clients are going to be attended" << std::endl;
    cg->shop->close();
    pthread_exit(nullptr);

}