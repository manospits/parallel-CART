#!/bin/bash -l
#SBATCH -D ./
#SBATCH --export=ALL
#SBATCH --time 00:30:00

# load modules
module load compilers/intel/2019u5 mpi/intel-mpi/2019u5/bin

echo Compiling random forest
make ALL

echo Running random forest
time ./cart

echo Cleaning up
make clean