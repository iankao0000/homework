CC = gcc
CFLAGS = -O0 -Wall

TARGETS = multiple_coro coroutines

all: $(TARGETS)

multiple_coro: 
	$(CC) $(CFLAGS) -o multiple_coro ./example/multiple_coro.c neco.o
coroutines: 
	$(CC) $(CFLAGS) -o coroutines ./example/coroutines.c neco.o
clean:
	rm -f $(TARGETS)
