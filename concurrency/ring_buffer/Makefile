main:
	gcc -o main test_dyn.c -lpthread
	./main > input.txt
check:
	python3 verify.py input.txt
clean:
	rm main input.txt
