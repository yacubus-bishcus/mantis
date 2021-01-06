echo Configuring mantis...
echo Please Enter CERN ROOT build directory 
read -p "The ROOT Directory: " INVAR1
echo Please Enter Geant4 install directory 
read -p "The Geant4 Directory: " INVAR2
current_dir="$(pwd)"
cd ~ && mkdir MANTIS_MAIN_DIR && mv $current_dir MANTIS_MAIN_DIR && cd MANTIS_MAIN_DIR/mantis
tar xfz NRF_Database.tar.gz && mv Database1.1 ../ && cd ../Database1.1
database_working_dir="$(pwd)"
echo "Exporting the Database working directory path: $database_working_dir"

VAR1="/etc/cmake"
VARCMAKEPATH=$INVAR1$VAR1
echo "Exporting the ROOT CERN Cmake path: $VARCMAKEPATH"
VAR2="/bin/thisroot.sh"
VARROOTBIN=$INVAR1$VAR2
echo "Sourcing $VARROOTBIN"
VAR3="/geant4.sh"
VARGEANTBIN=$INVAR2$VAR3
echo "Sourcing $VARGEANTBIN"
VAR4="/share/Geant4-10.5.1/geant4make/geant4make.sh"
VARGEANTMAKE=$INVAR2$VAR4
echo "Sourcing $VARGEANTMAKE"

echo Building Mantis...
cd ../ && mkdir mantis_run && cd mantis_run
