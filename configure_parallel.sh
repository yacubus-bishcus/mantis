#!/bin/bash

#// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#// Author:
#// Jacob E Bickus, 2021
#// MIT, NSE
#// jbickus@mit.edu
#// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#///////////////////////////////////////////////////////////////////////////////

mkdir lib
. ./setup.sh

PAR_TARGET=mantis_mpi
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
cp mantis.cc mantis_mpi.cc
# If the parallel binary exists, remove it
if [ -a $BINARY ]; then rm $BINARY; fi

# Get the number of available effective cores

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

make -j$CORES

# Test to ensure that the parallel binary was built successfully
if [ -a $BINARY ]
    then
    echo -e '\n************************************************************'
    echo -e '   Mantis was successfully built in parallel for '$CORES' cores!'
    echo -e '                MPI parallelization enabled!'
    echo -e '************************************************************\n'
fi

# Remove the parallel implementation file
rm -f $PAR_TARGET.cc
