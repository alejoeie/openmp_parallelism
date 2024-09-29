#!/bin/bash
#SBATCH --job-name=mergesort
#SBATCH --output=mergesort.o%j
#SBATCH --partition=nu-wide
#SBATCH --ntasks=16
#SBATCH --time=00:05:00

cd $SLURM_SUBMIT_DIR

make all
./test.exe 10 100000