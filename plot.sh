#! /usr/bin/bash
./mandel -l|gnuplot -p -e 'set view map; set dgrid3d; plot "/dev/stdin" using 1:2:3 with image'