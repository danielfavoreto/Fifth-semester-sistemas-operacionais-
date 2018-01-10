#include <iostream>
#include "BarberGenerator.h"

// constructor
BarberGenerator::BarberGenerator(BarberShop *bs, unsigned int max): shop(bs), maxBarbersTG(max) {}

// destructor
BarberGenerator::~BarberGenerator() {
    shop = nullptr;
}

// client generating limit
unsigned int BarberGenerator::getMaxBarbersTG() {
    return maxBarbersTG;
}

// the thread start method
void BarberGenerator::start() {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, run, this);
    return;
}

void BarberGenerator::joinBarberGeneratorThread() {
    pthread_join(tid, nullptr);
}

// overriding Runnable run() method
void* BarberGenerator::run(void *arg) {

    BarberGenerator *cg = (BarberGenerator*) arg;
    if (nullptr == cg) {
        std::cout << "Error! Invalid BarberGenerator pointer";
        pthread_exit(nullptr);
    }

    unsigned int i = 0;

    Barber *b = nullptr;
    // see maxBarbers
    while(i < cg->getMaxBarbersTG()) {

        // create a Barber Thread
        b = new Barber(cg->shop, i+1);
        b->start();
        // see maxBarbers
        i++;
    }

    b = nullptr;
    pthread_exit(nullptr);
}

