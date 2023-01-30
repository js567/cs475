#!/bin/bash
#SBATCH -J AutoCorr
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH -N 10 # number of nodes
#SBATCH -n 10 # number of tasks
#SBATCH --constraint=ib
#SBATCH -o autocorr.out
#SBATCH -e autocorr10.err
module load openmpi
mpic++ p7b.cpp -o autocorr -lm
mpiexec -mca btl self,tcp -np 10 ./autocorr