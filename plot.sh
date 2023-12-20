#! /usr/bin/bash
# ./mandel -r 1920 -i 1080 |gnuplot -p -e 'set view map; set dgrid3d; plot "/dev/stdin" using 1:2:5 with image; pause mouse close'

input_file=./mandel.out
./mandel -r 1920 -i 1080 > $input_file
gnuplot -p -e "set view map; set dgrid3d; plot './mandel.out' using 1:2:5 with image; pause mouse close"