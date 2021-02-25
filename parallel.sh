#!/bin/bash



#// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#// Author:
#// Jacob E Bickus, 2021
#// MIT, NSE
#// jbickus@mit.edu
#// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#///////////////////////////////////////////////////////////////////////////////
PAR_TARGET=mpi_mantis

BINARY=$G4WORKDIR/bin/$G4SYSTEM/$PAR_TARGET
BUILDDIR=$G4WORKDIR/tmp/$G4SYSTEM/$PAR_TARGET

# Remove the old binary
if [ -a $BINARY ]
    then
    rm $BINARY
fi

# Command-line capability to clean the build directory
if [ "$1" == 'clean' ];
then
    echo -e "\nCleaning up $PAR_TARGET build directory ...\n"
    rm -rf $BUILDDIR
    exit
else
    echo -e "\nBuilding the parallel version of mantis ...\n"
fi

if [[ "$OSTYPE" == "linux-gnu" ]]; then
        # linux
    CORES=$(grep -c ^processor /proc/cpuinfo)
elif [[ "$OSTYPE" == "darwin"* ]]; then
        # Mac OSX
    CORES=$(sysctl -a | grep core_count | sed 's/machdep.cpu.core_count: //')
elif [[ "$OSTYPE" == "freebsd"* ]]; then
        # ...
    CORES=$(sysctl -a | grep core_count | sed 's/machdep.cpu.core_count: //')
else
     echo 'Unknown OS';
     CORES=-1;
fi

cd ../
mkdir RunMantis
cd RunMantis
cp ../mantis/mantis.cc ../mantis/mpi_mantis.cc
mkdir tmp
cmake -DWITH_MPI=ON -DGeant4_DIR=/home/jbickus/geant4/lib64/Geant4-10.5.1 -DCMAKE_CXX_COMPILER=mpicxx -DG4mpi_DIR=/home/jbickus/GEANT4MPI/install/lib64/G4mpi-10.5.1 ../mantis
echo "Compiling with $CORES cores..."
make -j$CORES

cd ../mantis && rm mpi_mantis.cc