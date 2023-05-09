#!/bin/bash -l

#SBATCH -J task_1_1
#SBATCH -t 01:00:00
#SBATCH -A edu23.DD2356
#SBATCH -p shared
#SBATCH -e task_1_1.e
#SBATCH -n 4

#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --nodes=4

srun ./task_1_1.out > task_1_1.output