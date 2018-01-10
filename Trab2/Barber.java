import java.util.*;
import java.lang.*;

public class Barber implements Runnable {

    // unique barber id
    long myid;

    // Barber shop 
    BarberShop shop;

    // constructor
    public Barber(BarberShop bs, long id) {
        myid = id;
        shop = bs;
    }

    // overriding run method
    public void run() {

        System.out.println("\nBarber " + myid + " starting to shaving everybody!");

        Client client;
        while(shop.isOpen() || shop.isThereClient()) {
            synchronized (shop.clients) {
                // getThreadClient() is a sync method
                client = shop.getThreadClient();
                if (null == client && shop.isOpen()) {
                    try {
                        System.out.println("\nBarber " + myid + " says: nobody here! Let's take a nap!");
                        shop.clients.wait();
                    } catch (InterruptedException ieerror) {
                        System.out.println("Error: " + ieerror);
                    }
                }
            }


            if (null != client) {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException ieerror) {
                    System.out.println("Error: " + ieerror);
                }
                synchronized(client) {
                    try {
                        client.cutHair();
                        System.out.println("\nBarber " + myid + "! Done with client " + client.getId() + "!");
                        client.notify();

                    } catch (IllegalMonitorStateException ilerror) {
                        System.out.println("Error: " + ilerror);
                    }
                }
            }
       }
        System.out.println("\nBarber " + myid + " says: good job for me!");
    }

}