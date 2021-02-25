#!/bin/bash



#// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#// Author:
#// Jacob E Bickus, 2021
#// MIT, NSE
#// jbickus@mit.edu
#// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#///////////////////////////////////////////////////////////////////////////////

cp ../mantis/mantis.cc ../mantis/mantis_mpi.cc
mkdir tmp
cmake -DWITH_MPI=ON -DGeant4_DIR=/home/jbickus/geant4/lib[64]/Geant4-10.5.1 -DG4mpi_DIR=/home/jbickus/GEANT4MPI/install/lib64/G4mpi-10.5.1 ../mantis
make -j4
