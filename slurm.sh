#!/bin/bash

#SBATCH --nodes=4
#SBATCH --ntasks-per-node=1
#SBATCH --constraint=IB|OPA
#SBATCH --time=00:10:00
#SBATCH --job-name="subsetsum-job"
#SBATCH --output=../output/output.out
#SBATCH --partition=general-compute
#SBATCH --qos=general-compute
#SBATCH --cluster=ub-hpc
#SBATCH --exclusive 

module load intel
# module list

# export I_MPI_DEBUG=4
export I_MPI_PMI_LIBRARY=/opt/software/slurm/lib64/libpmi.so

mpicc -o ../classfile/subsetsum ../subsetsum.c
mpirun -np 4 ../classfile/subsetsum
# srun -np 4 ../classfile/subsetsum
