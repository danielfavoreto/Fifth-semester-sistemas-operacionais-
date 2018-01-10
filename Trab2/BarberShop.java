import java.util.*;
import java.lang.*;

public class BarberShop {

    // Client linked list to be used as a queue
    LinkedList<Client> clients;
    int maxClients;

    // Barber linked list, to be used as a dynamic data
    LinkedList<Thread> barbers;

    // flag about the shop status
    Boolean open;

    // constructor
    public BarberShop(int max) {
        clients = new LinkedList<Client>();
        barbers = new LinkedList<Thread>();
        open = true;
        maxClients = max;
    }

    // add a new Barber Thread to barbers LinkedList
    public synchronized void addThreadBarber(Thread b) {
        barbers.add(b);
    }

    // get the first Barber Thread and remove it from the linked list
    public synchronized Thread getThreadBarber() {
        return barbers.poll();
    }

    // add a new Client Thread to clients LinkedList
    public synchronized boolean addThreadClient(Client c) {
        if (maxClients > clients.size()) {
            clients.add(c);
            return true;
        }
        return false;
    }

    
    // get the first Client Thread and remove it from the linked list
    public synchronized Client getThreadClient() {
        Client c = clients.poll();
        if (null != c) {
            System.out.println("\nClient " + c.getId() + " attended!");
        }
        return c;
    }

    // notify a Barber about client linked list update
    public synchronized void notifyBarber() {
        synchronized (clients) {
            try {
                clients.notify();
            } catch (IllegalMonitorStateException error) {
                System.out.println("Error de: " + error);
            }
        }
    }

    // notify all Barbers about client linked list
    public synchronized void notifyAllBarbers() {
        synchronized (clients) {
            try {
                clients.notifyAll();
            } catch (IllegalMonitorStateException error) {
                System.out.println("Error de: " + error);
            }
        }
    }

    // is really open?
    public synchronized boolean isOpen() {
        return open;
    }

    // close the shop
    public synchronized void close() {
        open = false;
        System.out.println("\nWe are closing, but we are going to attend all " + clients.size() + " remaining clients!");
        // notify all Barbers about this info
        notifyAllBarbers();
    }

    // is an empty linked list?
    public synchronized boolean isThereClient() {
        return clients.size() > 0;
    }
 }