#!/bin/bash

clear

for i in {0..8}; do
  task=$((2**i))
  # task=$i
  rm ../output/output-$task.out
  rm ../output/error-output-$task.out
  sbatch --nodes=$task --job-name="subsetsum-job-$task" --output=../output/output-$task.out --error=../output/error-output-$task.out slurm.sh $task
done
