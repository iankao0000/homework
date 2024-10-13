CC = gcc
CFLAGS = -O0 -Wall

TARGETS = ticktock coroutines

all: $(TARGETS)

ticktock: 
	$(CC) $(CFLAGS) -o ticktock ./example/ticktock.c neco.o
coroutines: 
	$(CC) $(CFLAGS) -o coroutines ./example/coroutines.c neco.o
clean:
	rm -f $(TARGETS)
