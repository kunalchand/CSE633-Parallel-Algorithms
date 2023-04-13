#!/bin/bash
# module avail intel-mp
module load intel-mpi/2020.2
module load intel/17.0
clear
mpicc -o myprogram myprogram.c
read -p "Enter the number of processors: " np
mpirun -np $np ./myprogram