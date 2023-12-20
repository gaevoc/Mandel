#! /usr/bin/bash
./mandel -r 1920 -i 1080 -l|gnuplot -p -e 'set view map; set dgrid3d; plot "/dev/stdin" using 1:2:3 with image'