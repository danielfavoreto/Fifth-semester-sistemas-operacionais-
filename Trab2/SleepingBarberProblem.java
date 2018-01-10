import java.util.*;

public class SleepingBarberProblem {

    // main method, default
    public static void main (String[] args) {

        // number of barbers and clients
        int maxBarbers, chairs;

        // get args, if any
        if (1 < args.length) {
            // get first arg
            int parsedInt = Integer.parseInt(args[0]);
            if (0 < parsedInt) {
                maxBarbers = parsedInt;
            } else {
                maxBarbers = 3;
            }
            // get second arg
            parsedInt = Integer.parseInt(args[1]);
            if (0 < parsedInt) {
                chairs = parsedInt;
            } else {
                chairs = 15;
            }
        } else {
            maxBarbers = 3;
            chairs = 15;
        }

        // creates a new BarberShop
        BarberShop shop = new BarberShop(chairs);

        // add the new barbers to the shop
        for (int i = 0; i < maxBarbers; i++) {
            // create new Barber Thread
            Thread b = new Thread(new Barber(shop, i+1));

            // add Barber Thread to BarberShop barbers list
            shop.addThreadBarber(b);

            // run Barber Thread
            b.start();
        }

        // instantiate a random Client generator
        Thread addClients = new Thread(new ClientGenerator(shop, 2*(chairs + maxBarbers)));

        // startst to run the Client generator
        addClients.start();

        try {
            // wait for the end of Client generator
            addClients.join();
        } catch (InterruptedException ieerror){
            System.out.println("Error " + ieerror);
        }

        // close the shop
        shop.close();

        // join all barbers
        for (int i = 0; i < maxBarbers; i++) {
            Thread b = shop.getThreadBarber();
            if (null != b) {
                try {

                    // join BarberThread
                    b.join();

                } catch (InterruptedException ieerror){
                    System.out.println("Error " + ieerror);
                }
            } else {
                System.out.println("Error! Invalid join!");
            }
        }
        return;
    }
}