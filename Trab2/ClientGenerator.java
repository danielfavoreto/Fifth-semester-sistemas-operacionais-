import java.util.*;
// internal class, a Client object factory
public class ClientGenerator implements Runnable {

    // client threads list
    LinkedList<Thread> ts;

    // the shop
    BarberShop shop;

    // number of customers
    int maxClients;

    // constructor
    public ClientGenerator(BarberShop bs, int max) {
        shop = bs;
        maxClients = max;
        ts = new LinkedList<Thread>();
    }

    // overriding Runnable run() method
    @Override
    public void run() {
        System.out.println("\nHaired clients comming!!");

        // randon integer generator
        Random rand = new Random();

        int i = 0;

        // see maxClients
        while(i < maxClients*2) {

            // create a Client Thread
            Client c = new Client(shop, i+1);
            Thread t = new Thread(c);

            // add Thread to BarberShop clients list
            ts.add(t);
            // run the Client Thread
            t.start();

            // try to sleep 400 ms
            try {
                Thread.sleep(rand.nextInt(400));
            } catch (InterruptedException ieerror){
                System.out.println("Error: " + ieerror);
            }
            // see maxClients
            i++;
        }

        // join all client threads
        for (i = 0; i < ts.size(); i++) {
            Thread t = ts.poll();
            if (null != t) {
                try {
                    // wait for Client Thread
                    t.join();
                } catch (InterruptedException ieerror) {
                    System.out.println("Error: " + ieerror);
                }
            }
        }
    }
}
