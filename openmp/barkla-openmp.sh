#!/bin/bash -l

### COMP328 (lab03): example batch script
###           usage: "sbatch run.sh"
###         purpose: to run serial and then parallel OpenMP & MPI on requested number of cores
###    restrictions: this script can only handle single nodes
### (c) mkbane, University of Liverpool (2020)

# Specific course queue
#SBATCH -p course

# Defaults on Barkla (but set to be safe)
## Specify the current working directory as the location for executables/files
#SBATCH -D ./
## Export the current environment to the compute node
#SBATCH --export=ALL

# load modules
## intel compiler
module load compilers/intel/2019u5 
## intel mpi wrapper and run time
module load mpi/intel-mpi/2019u5/bin

# SLURM terms
## nodes            relates to number of nodes
## ntasks-per-node  relates to MPI processes per node
## cpus-per-task    relates to OpenMP threads (per MPI process)

# determine number of cores requested (NB this is single node implementation)
## further options available via examples: /opt/apps/Slurm_Examples/sbatch*sh
echo "Node list                    : $SLURM_JOB_NODELIST"
echo "Number of nodes allocated    : $SLURM_JOB_NUM_NODES or $SLURM_NNODES"
echo "Number of threads or processes          : $SLURM_NTASKS"
echo "Number of processes per node : $SLURM_TASKS_PER_NODE"
echo "Requested tasks per node     : $SLURM_NTASKS_PER_NODE"
echo "Requested CPUs per task      : $SLURM_CPUS_PER_TASK"
echo "Scheduling priority          : $SLURM_PRIO_PROCESS"

# check expected inputs (OpenMP is only supported on a single node)
if [ "$SLURM_NNODES" -gt "1" ]; then 
    echo more than 1 node not allowed
    exit
fi

echo Compiling random forest
make ALL

export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-1} # if '-c' not used then default to 1
echo using ${OMP_NUM_THREADS} OpenMP threads
time ./cart

echo Cleaning up
make clean
