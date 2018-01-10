#include <iostream>
#include <pthread.h>

#include "Client.h"
#include "Barber.h"
#include "BarberShop.h"
#include "ClientGenerator.h"
#include "BarberGenerator.h"


int main() {

    BarberShop bs(3, 15);

    // the Barber generator pthread
    BarberGenerator bg(&bs, bs.getMaxBarbers());
    // start the own object thread!!
    bg.start();

    // the client generator pthread
    ClientGenerator cg(&bs, 2*(bs.getMaxBarbers() + bs.getMaxClients()));
    // start the own object thread!!
    cg.start();

    // wait all threads

    // wait this thread work
    cg.joinClientGeneratorThread();
    bg.joinBarberGeneratorThread();
    bs.joinLostClientsAndBarbers(); 
    return 0;
}

