#!/bin/bash
#SBATCH -N 1
#SBATCH -n 1
#SBATCH --time=0:10:00
#SBATCH -p newnodes
#SBATCH --job-name=GEANT4
MACRO=mantis.in
OUTFILENAME=$OUTFILE
ARG=$envvar_bsh
srun ./mantis -m ${MACRO} -o ${OUTFILENAME}-${ARG}.root -s ${ARG} &
wait
exit
