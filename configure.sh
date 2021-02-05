#!/bin/bash

# Default Arguments 
GEANT4_DIR = "None"
ROOT_CERN_DIR = "None"
for arg in "$@"
do 
   case $arg in
        -h|--help)
        echo "Configure Script for Mantis"
        echo "options:"
        echo "-h, --help  show brief help"
        echo "-g,         specify the Geant4 Install Directory (mandatory)"
        echo "-r,         specify thisroot.sh Directory (optional)"
        exit 0
        ;;
        shift # Remove --initialize from processing
        ;;
        -g|--geant4_dir=*)
        GEANT4_DIR="$1"
        shift 
        ;;
        -r|--root_dir)
        ROOT_DIRECTORY="$2"
        shift # Remove argument name from processing
        ;;
    esac
done

echo "Geant4 Directory: $GEANT4_DIR"
echo "ROOT Directory: $ROOT_DIRECTORY"

if([$GEANT4_DIR = "None"])
    exit 0
fi

echo Configuring mantis...
echo Using the following ROOT CERN Directory...
which "root"
VAR1=$(which "root")
VAR2=${VAR1:0:${#VAR1}-4}
current_dir="$(pwd)"
cd ~ && mkdir MANTIS_MAIN_DIR && mv $current_dir MANTIS_MAIN_DIR && cd MANTIS_MAIN_DIR/mantis
tar xfz NRF_Database.tar.gz && mv Database1.1 ../ && cd ../Database1.1
database_working_dir="$(pwd)"
echo "Exporting the Database working directory path: $database_working_dir"
export G4NRFGAMMADATA=$database_working_dir

if($ROOT_DIRECTORY != "None")
    VARROOTBIN=$ROOT_DIRECTORY
else
    VAR3="thisroot.sh"
    VARROOTBIN=$VAR2$VAR3
fi

echo "Sourcing $VARROOTBIN"
source $VARROOTBIN
VAR4="/geant4.sh"
VARGEANTBIN=$GEANT4_DIR$VAR4
echo "Sourcing $VARGEANTBIN"
source $VARGEANTBIN
VAR5="/share/Geant4-10.5.1/geant4make/geant4make.sh"
VARGEANTMAKE=$GEANT4_DIR$VAR5
echo "Sourcing $VARGEANTMAKE"
source $VARGEANTMAKE

echo Building Mantis...
cd ../ && mkdir mantis_run && cd mantis_run && cmake ../mantis && make -j4 && cd ../mantis/Input_Files

echo Mantis built.
echo Creating Default brems_distributions.root
root -b -q -l 'Sampling.cc("Brem2.1_100M.root",2.1,"U")'
cp brems_distributions.root ../../mantis_run && cd ../../mantis_run

./mantis -m mantis.in -o test.root -s 1 

echo "Mantis Configured and Test Run Complete. Test results can be found in test_error.log and test.log. Good Luck and try the README.md!"
