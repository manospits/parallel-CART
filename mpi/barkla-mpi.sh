#!/bin/bash -l
#SBATCH -D ./
#SBATCH --export=ALL
#SBATCH --time 01:30:00
#SBATCH --exclusive

# load modules
module load compilers/intel/2019u5 
module load mpi/intel-mpi/2019u5/bin

echo "Node list                    : $SLURM_JOB_NODELIST"
echo "Number of nodes allocated    : $SLURM_JOB_NUM_NODES or $SLURM_NNODES"
echo "Number of threads or processes          : $SLURM_NTASKS"
echo "Number of processes per node : $SLURM_TASKS_PER_NODE"
echo "Requested tasks per node     : $SLURM_NTASKS_PER_NODE"
echo "Requested CPUs per task      : $SLURM_CPUS_PER_TASK"
echo "Scheduling priority          : $SLURM_PRIO_PROCESS"

echo Compiling random forest
make ALL

echo Training random forest
n_procs=${SLURM_NTASKS:-1}  # if no '-n' then we set this to 1 
time mpirun -np ${n_procs} ./cart

echo Cleaning up
make clean