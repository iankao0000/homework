CC = gcc
CFLAGS = -Wall -O3 -D__i386__

mountain: mountain.c fcyc2.c clock.c
	$(CC) $(CFLAGS) -o mountain mountain.c fcyc2.c clock.c 
	./mountain > data.dat

clean:
	rm -f mountain *.o *~ *.png *.dat
plot:
	gnuplot memory_mountain_script.gp

