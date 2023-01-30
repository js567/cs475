#!/bin/bash
for t in 1024 2048 4096 8192 16384 65536 131072 262144 524288 1048576 2097152 4194304 8388608
do
        for b in 32 64 128 256 
        do
                # /usr/local/apps/cuda/cuda-10.1/bin/nvcc -DNUMTRIALS=$t -DBLOCKSIZE=$b -o proj05  proj05.cu
                g++ -DNMB=$t -DLOCAL_SIZE=$b -o first first.cpp /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
                # ./proj05
                ./first
        done
done