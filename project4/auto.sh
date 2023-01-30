#!/bin/bash
# number of threads:
for a in 1000 2000 5000 10000 15000 25000 50000 100000 250000 500000 750000 1000000 1500000 2000000 2500000 3000000 4000000 5000000 6000000 7000000 8000000
do
    # echo NUMS = $s
    g++   all04.cpp  -DARRAYSIZE=$a  -o p4  -lm  -fopenmp
    ./p4
    
done
