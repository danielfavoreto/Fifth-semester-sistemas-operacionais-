PROG = SleepingBarber
CC = g++
CPPFLAGS = -pedantic -W -Wall -std=c++0x -pthread
OBJS = trab2.o Client.o ClientGenerator.o Barber.o BarberGenerator.o BarberShop.o

$(PROG): $(OBJS)
	$(CC) $(CPPFLAGS) -o $(PROG) $(OBJS)

trab2.o:
	$(CC) $(CPPFLAGS) -c trab2.cpp

client.o:
	$(CC) $(CPPFLAGS) -c Client.cpp

ClientGenerator.o:
	$(CC) $(CPPFLAGS) -c ClientGenerator.cpp

Barber.o:
	$(CC) $(CPPFLAGS) -c Barber.cpp

BarberGenerator.o:
	$(CC) $(CPPFLAGS) -c BarberGenerator.cpp

BarberShop.o:
	$(CC) $(CPPFLAGS) -c BarberShop.cpp

clean:
	rm -rf core $(PROG) $(OBJS)
