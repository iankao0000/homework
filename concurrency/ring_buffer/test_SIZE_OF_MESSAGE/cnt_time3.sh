#!/bin/bash

rm  output_size_1
rm  output_size_10
rm  output_size_100
rm  output_size_500
rm  ringbuffer3.png

gcc -o size_1 -pthread mmap_size_1.c
gcc -o size_10 -pthread mmap_size_10.c
gcc -o size_100 -pthread mmap_size_100.c
gcc -o size_500 -pthread mmap_size_500.c

for i in {128..65536..512};
do
    ./size_1 $i >> output_size_1;
    ./size_10 $i >> output_size_10;
    ./size_100 $i >> output_size_100;
    ./size_500 $i >> output_size_500;
done

gnuplot compare3.gp
