#!/bin/bash

# Default Arguments
INFILE="DEFAULT"
INTREEOBJ="DEFAULT"
OUTHISTO="hOut"
NBINS=420000
EMAX=2.1
NRFWIDTH=5e-6
CUTE=1.5
NNRFWIDTH=5e-3
VARARRAY="false"
ENERGYC="NA"

# Read Input Flags

for arg in "$@"
do
   case $arg in
        -h|--help)
        echo "Rebinning Script for Mantis Output"
        echo "options:"
        echo "-h,--help                 show brief help"
        echo "--read_file               specify the file to be read in to rebin"
        echo "--TTree_name              specify the TTree Object Name to be rebinned"
        echo "--hOut_name=hOut          specify the TH1D Object Name to be Created"
        echo "--nbins=420000            specify the number of bins in the TH1D to be Created"
        echo "--Emax=2.1                specify the Max Energy (Xaxis) of the TH1D to be Created"
        echo "-v,--var_array=false      specify if the TH1D will have variable bin widths"
        echo "--nrf_bin_width=5e-6      specify the bin width of the NRF region (eV)"
        echo "--cut_energy=1.5          specify the energy to begin the new NRF bin width"
        echo "--non_nrf_bin_width       specify the bin width of the non NRF region (eV)"
        echo "--energy_cut              specify the energy cut to place for rebinning (MeV)"
        shift
        exit 0
        ;;
        -a|--read_file=*)
        INFILE="${arg#*=}"
        shift
        ;;
        -b|--TTree_name=*)
        INTREEOBJ="${arg#*=}"
        shift # Remove argument name from processing
        ;;
       -c|--hOut_name=*)
       OUTHISTO="${arg#*=}"
       shift
       ;;
       -d|--nbins=*)
       NBINS="${arg#*=}"
       shift
       ;;
       -e|--Emax=*)
       EMAX="${arg#*=}"
       shift
       ;;
       -f|--nrf_bin_width=*)
       NRFWIDTH="${arg#*=}"
       shift
       ;;
       -g|--cut_energy=*)
       CUTE="${arg#*=}"
       shift
       ;;
       -j|--non_nrf_bin_width=*)
       NNRFWIDTH="${arg#*=}"
       shift
       ;;
       -k|--energy_cut=*)
       ENERGYC="${arg#*=}"
       shift
       ;;
       -v|--var_array=*)
       VARARRAY="true"
       VARARRAY="${arg#*=}"
       shift
       ;;
    esac
done

echo Running Rebin with parameters:
echo File to read: $INFILE
echo TTree to analyze: $INTREEOBJ
echo Histo Name: $OUTHISTO
echo Number of Bins: $NBINS
echo Max Histo Energy: $EMAX
echo Variable Bins: $VARARRAY
echo NRF Bin Width: $NRFWIDTH
echo Non NRF Bin Width: $NNRFWIDTH
echo Energy Region Cut: $CUTE
echo Histogram Cut: $ENERGYC

# Conduct Input Check
if [ $INFILE == "DEFAULT" ]
then
  echo ERROR USER MUST INPUT INFILE AND TTREE OBJECT TO READ
  echo RUN ./Rebin.sh -h for help!
  exit 0
fi

if [ $INTREEOBJ == "DEFAULT" ]
then
  echo ERROR USER MUST INPUT INFILE AND TTREE OBJECT TO READ
  echo RUN ./Rebin.sh -h for help!
  exit 0
fi

# Check Complete Run Analysis

# Construct input argument
CVAR1="'Rebin.cc("
CVAR2=")'"
QVAR="\""
COM=","
VCOMMANDWOCUT=$CVAR1$QVAR$INFILE$QVAR$COM$QVAR$INTREEOBJ$QVAR$COM$QVAR$OUTHISTO$QVAR$COM$NBINS$COM$EMAX$COM$VARARRAY$COM$NRFWIDTH$COM$NNRFWIDTH$COM$CUTE
if [ $ENERGYC != "NA" ]
then
  VCOMMAND=$VCOMMANDWOCUT$COM$ENERGYC$CVAR2
else
  VCOMMAND=$VCOMMANDWOCUT$CVAR2
fi

echo Running root -b -q $VCOMMAND

#root -l -b $VCOMMAND
