#!/bin/bash

#SBATCH --nodes=35
#SBATCH --ntasks-per-node=1
#SBATCH --constraint=IB|OPA
#SBATCH --time=00:10:00
#SBATCH --partition=general-compute
#SBATCH --qos=general-compute
#SBATCH --mail-type=END
#SBATCH --mail-user=kchand@buffalo.edu
#SBATCH --job-name="subsetsum-job-35"
#SBATCH --output=../output/output-35.out
#SBATCH --exclusive 

module load intel
module list

export I_MPI_DEBUG=4
export I_MPI_PMI_LIBRARY=/opt/software/slurm/lib64/libpmi.so

mpicc -o ../classfile/subsetsum ../subsetsum.c
# mpirun -np 3535 ../classfile/subsetsum
srun -n 35 ../classfile/subsetsum
