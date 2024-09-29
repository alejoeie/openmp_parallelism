#!/bin/bash
#SBATCH --job-name=mergesort
#SBATCH --output=mergesort.o%j
#SBATCH --partition=nu-wide
#SBATCH --ntasks=16
#SBATCH --time=00:05:00

module load gcc/7.2.0
module load mpich/3.2.1-gcc-7.2.0
module load cmake/3.17.2

export CILK_NWORKERS = 16
cd $SLURM_SUBMIT_DIR

make all_cilk