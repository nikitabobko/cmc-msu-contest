CC = gcc
CFLAGS = -Wall -O2
LDLIBS = -lm

all: genrand

genrand: main.o rand.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f *.o genrand