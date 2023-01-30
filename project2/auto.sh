#!/bin/bash
# number of threads:
for t in 1 2 4 8 16 20 32
do
    # echo NUMT = $t
    # number of subdivisions:
    for n in 100 500 1000 1500 3000  
    do
        # echo NUMS = $s
        g++ -O3   proj02.cpp  -DNUMT=$t -DNUMNODES=$n  -o proj02  -lm  -fopenmp
        ./proj02
    done
done
