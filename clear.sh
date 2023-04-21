#!/bin/bash

clear

for task in {5..10}; do
  rm ../output/output-$task.out
  rm ../output/error-output-$task.out
  sbatch --nodes=$task --job-name="subsetsum-job-$task" --output=../output/output-$task.out --error=../output/error-output-$task.out slurm.sh $task
done
