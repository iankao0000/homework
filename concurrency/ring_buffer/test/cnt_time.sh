#!/bin/bash

rm  output_dyn
rm  output_mmap
rm  ringbuffer.png

gcc -o test_dyn -pthread test_dyn.c
gcc -o test_mmap -pthread test_mmap.c

for i in {128..65536..128};
do
    ./test_dyn $i >> output_dyn;
    ./test_mmap $i >> output_mmap;
done

gnuplot compare.gp
