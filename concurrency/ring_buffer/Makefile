main:
	gcc -o dyn_ring ./src/test_dyn.c -lpthread
	gcc -Wall -std=c11 -D_GNU_SOURCE -o mmap_ring ./src/test_mmap.c -lpthread
check_dyn:
	./dyn_ring > input.txt
	python3 verify.py input.txt
check_mmap:
	./mmap_ring > input.txt
	python3 verify.py input.txt
clean:
	rm *ring input.txt
