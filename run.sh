#!/bin/bash

module load intel
# clear
mpicc -o ../classfile/subsetsum ../subsetsum.c
read -p "Enter the number of processors: " np
mpirun -np $np ../classfile/subsetsum
