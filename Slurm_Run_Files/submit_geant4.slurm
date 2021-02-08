#!/bin/bash
#SBATCH -N 1
#SBATCH -n 1
#SBATCH --time=6:00:00
#SBATCH -p newnodes
#SBATCH --job-name=$1
#SBATCH --constraint=centos7
MACRO=$1
OUTFILENAME=$OUTFILE
ARG=$envvar_bsh
srun ./mantis -m ${MACRO} -o ${OUTFILENAME}-${ARG}.root -s ${ARG} &
wait
exit
