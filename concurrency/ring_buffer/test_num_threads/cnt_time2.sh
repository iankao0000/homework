#!/bin/bash

rm  output_mmap
rm  output_mmap2
rm  output_mmap4
rm  ringbuffer2.png

gcc -o test_mmap -pthread test_mmap.c
gcc -o test_mmap2 -pthread test_mmap2.c
gcc -o test_mmap4 -pthread test_mmap4.c

for i in {128..65536..128};
do
    ./test_mmap $i >> output_mmap;
    ./test_mmap2 $i >> output_mmap2;
    ./test_mmap4 $i >> output_mmap4;
done

gnuplot compare2.gp
