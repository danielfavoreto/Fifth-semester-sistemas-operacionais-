public class Client implements Runnable {

    // Unique Client id
    long myid;

    // haired flag
    Boolean haired;

    // the actual shop
    BarberShop shop;

    // Client constructor
    public Client(BarberShop bs, long id) {
        myid = id;
        shop = bs;
        haired = true;
    }

    // overriding run method
    @Override
    public void run() {
        // add the current object to the BarberShop client list
        if(shop.addThreadClient(this)) {
            // notify all Barbers about the Client arriving object
            shop.notifyBarber();

            // wait until shaving or a nice hair cut
            while (haired) {
                System.out.println("\nI'm the client " + myid + ". I'm entering the BarberShop and I'm waiting for a hair cut!");
                synchronized (this) {
                    try {
                        // wait self object, naive?
                        this.wait();

                    } catch (InterruptedException ieerror) {
                        System.out.println("Error: " + ieerror);

                    }
                }
            }
            
        } else {
            System.out.println("\nCrowded here!! I'm client " + myid + ". Let's try another BarberShop!");
        }
        

    }

    // get the Client id
    public long getId() {
        return myid;
    }

    // set Haired Boolean value to false
    public synchronized void cutHair() {
        haired = false;
    }
}