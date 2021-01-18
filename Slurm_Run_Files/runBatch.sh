#!/bin/bash

START=$1
LIMIT=$2
export OUTFILE=$3
for ((a=START; a<LIMIT;a++))
do
  export envvar_bsh=$a
  sbatch submit_geant4.slurm &
  sleep 0.2
done 
wait 
