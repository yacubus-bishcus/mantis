#!/bin/bash
INFILE="DEFAULT"
INFILE2="NA"
CHOPDATA="false"
INTOBJDATA="false"
NRFDATA="false"
WATERDATA="false"
CHERENKOVDATA="false"
DETDATA="true"
INCDETDATA="false"
PRINTDATA="false"
PRINTDATAONLY="false"
current_dir=pwd
file_var="/Rebin.cc"
FILECHECK=$current_dir$file_var

if test -e "$FILECHECK"; then
  echo "$FILECHECK found."
else
  echo "$FILECHECK not found."
  exit(1)
fi

for arg in "$@"
do
   case $arg in
        -h|--help)
        echo "Rebinning Script for Mantis Output"
        echo "options:"
        echo "-h,--help               show brief help"
        echo "--read_on_file          specify the on file to be read in to rebin"
        echo "--read_off_file=NA      specify the off file to be read"
        echo "--ChopData=false        specify chopper data analysis"
        echo "--IntObjData=false      specify Interrogation Object analysis"
        echo "--NRFData=false         specify NRF Data Analysis"
        echo "--WaterData=false       specify Water Data Analysis"
        echo "--CherenkovData=false   specify Cherenkov Data Analysis"
        echo "--DetData=true          specify Detected Data Analysis"
        echo "--IncDetData=false      specify Incident Detector Data Analysis"
        echo "--PrintData=false       Run PrintResults.cc"
        echo "--PrintDataOnly=false   Only Run Print Results.cc Do not Rebin!"
        shift
        exit 0
        ;;
        -a|--read_on_file=*)
        INFILE="${arg#*=}"
        shift
        ;;
        -b|--read_off_file=*)
        INFILE2="${arg#*=}"
        shift
        ;;
        -chop|--ChopData=*)
        CHOPDATA="${arg#*=}"
        shift # Remove argument name from processing
        ;;
       -obj|--IntObjData=*)
       INTOBJDATA="${arg#*=}"
       shift
       ;;
       -nrf|--NRFData=*)
       NRFDATA="${arg#*=}"
       shift
       ;;
       -water|--WaterData=*)
       WATERDATA="${arg#*=}"
       shift
       ;;
       -ch|--CherenkovData=*)
       CHERENKOVDATA="${arg#*=}"
       shift
       ;;
       -det|--DetData=*)
       DETDATA="${arg#*=}"
       shift
       ;;
       -IncDet|--IncDetData=*)
       INCDETDATA="${arg#*=}"
       shift
       ;;
       -Print|--PrintData=*)
       PRINTDATA="${arg#*=}"
       shift
       ;;
       -PrintOnly|--PrintDataOnly=*)
       PRINTDATAONLY="${arg#*=}"
       shift
       ;;
    esac
done

if [ $INFILE != "DEFAULT" ]
then
  echo "Chopper Data Set to: $CHOPDATA"
  echo "Interrogation Object Data: $INTOBJDATA"
  echo "NRF Data: $NRFDATA"
  echo "Water Data: $WATERDATA"
  echo "Cherenkov Data: $CHERENKOVDATA"
  echo "Detected Data: $DETDATA"
  echo "Incident Detector Data: $INCDETDATA"
  echo "Print Results: $PRINTDATA"
  echo "Print Results Only: $PRINTDATAONLY"

  if [ $PRINTDATAONLY == "false" ]
  then
    echo "Rebinning File: $INFILE"
  fi
else
  echo "USER ERROR: MUST INPUT File to Rebin!"
  exit
fi


if [ $INFILE2 != "NA" ] && [ $PRINTDATAONLY == "false" ]
then
  echo "Rebinning Off File: $INFILE2"
fi

VAR1="\""
if [ $PRINTDATAONLY == "false" ]
then

  theFile=$VAR1$INFILE$VAR1
  theFile2=$VAR1$INFILE2$VAR1

  if [ $CHOPDATA != "false" ] && [ $CHOPDATA != "False" ]
  then
    root -l -b -q "Rebin.cc($theFile,\"ChopIn\",\"hChopIn\",300)"
    root -l -b -q "Rebin.cc($theFile,\"ChopOut\",\"hChopOut\",300)"
    if [ $INFILE2 != "NA" ]
    then
      root -l -b -q "Rebin.cc($theFile2,\"ChopIn\",\"hChopIn\",300)"
      root -l -b -q "Rebin.cc($theFile2,\"ChopOut\",\"hChopOut\",300)"
    fi
  fi

  if [ $INTOBJDATA != "false" ] && [ $INTOBJDATA != "False" ]
  then
    # Rebin IntObj > 1.5 MeV with 600 bins bin width 1e-3
    root -l -b -q "Rebin.cc($theFile,\"IntObjIn\",\"hIntObjIn\",600,1.5,2.1,\"Energy>1.5\")"
    # Rebin IntObj > 1.5 MeV with 120,000 bins bin width 5e-6
    root -l -b -q "Rebin.cc($theFile,\"IntObjIn\",\"hIntObjIn\",120000,1.5,2.1,\"Energy>1.5\")"
    root -l -b -q "Rebin.cc($theFile,\"IntObjIn\",\"hIntObjIn\",300)"
    root -l -b -q "Rebin.cc($theFile,\"IntObjOut\",\"hIntObjOut\",300)"
    if [ $INFILE2 != "NA" ]
    then
      # Rebin IntObj > 1.5 MeV with 600 bins bin width 1e-3
      root -l -b -q "Rebin.cc($theFile2,\"IntObjIn\",\"hIntObjIn\",600,1.5,2.1,\"Energy>1.5\")"
      # Rebin IntObj > 1.5 MeV with 120,000 bins bin width 5e-6
      root -l -b -q "Rebin.cc($theFile2,\"IntObjIn\",\"hIntObjIn\",120000,1.5,2.1,\"Energy>1.5\")"
      root -l -b -q "Rebin.cc($theFile2,\"IntObjIn\",\"hIntObjIn\",300)"
      root -l -b -q "Rebin.cc($theFile2,\"IntObjOut\",\"hIntObjOut\",300)"
    fi
  fi

  if [ $NRFDATA != "false" ] && [ $NRFDATA != "False" ]
  then
    root -l -b -q "Rebin.cc($theFile,\"NRF\",\"hNRF\",300)"
    if [ $INFILE2 != "NA" ]
    then
      root -l -b -q "Rebin.cc($theFile2,\"NRF\",\"hNRF\",300)"
    fi
  fi

  if [ $WATERDATA != "false" ] && [ $WATERDATA != "False" ]
  then
    root -l -b -q "Rebin.cc($theFile,\"Water\",\"hWater\",300)"
    if [ $INFILE2 != "NA" ]
    then
      root -b -q "Rebin.cc($theFile2,\"Water\",\"hWater\",300)"
    fi
  fi

  if [ $CHERENKOVDATA != "false" ] && [ $CHERENKOVDATA != "False" ]
  then
    root -l -b -q "Rebin.cc($theFile,\"Cherenkov\",\"hCherenkov\",300)"
    if [ $INFILE2 != "NA" ]
    then
      root -l -b -q "Rebin.cc($theFile2,\"Cherenkov\",\"hCherenkov\",300)"
    fi
  fi

  if [ $DETDATA != "false" ] && [ $DETDATA != "False" ]
  then
    # Rebin Detected with 20 bins Energy < 5e-6 bin width 0.25e-6
    root -l -b -q "Rebin.cc($theFile,\"DetInfo\",\"hDet\",20,0.0,5e-6,\"Energy<5e-6\")"
    if [ $INFILE2 != "NA" ]
    then
      root -l -b -q "Rebin.cc($theFile2,\"DetInfo\",\"hDet\",20,0.0,5e-6,\"Energy<5e-6\")"
    fi
  fi

  if [ $INCDETDATA != "false" ] && [ $INCDETDATA != "False" ]
  then
    root -l -b -q "Rebin.cc($theFile,\"IncDetInfo\",\"hIncDetInfo\",300)"
    if [ $INFILE2 != "NA" ]
    then
      root -l -b -q "Rebin.cc($theFile2,\"IncDetInfo\",\"hIncDetInfo\",300)"
    fi
  fi

fi

if [ $INFILE2 != "NA" ] && [ $PRINTDATA != "false" ]
then
  VAR2="rebinned_300_"
  rebinnedFile=$VAR1$VAR2$INFILE$VAR1
  rebinnedFile2=$VAR1$VAR2$INFILE2$VAR1

  echo Running Z Test Analysis on $rebinnedFile and $rebinnedFile2

  if [ $CHOPDATA != "false" ] && [ $CHOPDATA != "False" ]
  then
    root -l -b -q "PrintResults.cc($rebinnedFile, $rebinnedFile2, \"hChopIn\")"
    root -l -b -q "PrintResults.cc($rebinnedFile, $rebinnedFile2, \"hChopOut\")"
  fi

  if [ $INTOBJDATA != "false" ] && [ $INTOBJDATA != "False" ]
  then
    root -l -b -q "PrintResults.cc($rebinnedFile, $rebinnedFile2, \"hIntObjIn\")"
    root -l -b -q "PrintResults.cc($rebinnedFile, $rebinnedFile2, \"hIntObjOut\")"
  fi

  if [ $NRFDATA != "false" ] && [ $NRFDATA != "False" ]
  then
    root -l -b -q "PrintResults.cc($rebinnedFile, $rebinnedFile2, \"hNRF\")"
  fi

  if [ $WATERDATA != "false" ] && [ $WATERDATA != "False" ]
  then
    root -l -b -q "PrintResults.cc($rebinnedFile, $rebinnedFile2, \"hWater\")"
  fi

  if [ $CHERENKOVDATA != "false" ] && [ $CHERENKOVDATA != "False" ]
  then
    root -l -b -q "PrintResults.cc($rebinnedFile, $rebinnedFile2, \"hCherenkov\")"
  fi

  if [ $INCDETDATA != "false" ] && [ $INCDETDATA != "False" ]
  then
    root -l -b -q "PrintResults.cc($rebinnedFile, $rebinnedFile2, \"hIncDetInfo\")"
  fi

  if [ $DETDATA != "false" ] && [ $DETDATA != "False" ]
  then
    VAR3="rebinned_20_"
    rebinnedDetFile=$VAR1$VAR3$INFILE$VAR1
    rebinnedDetFile2=$VAR1$VAR3$INFILE2$VAR1

    root -l -b -q "PrintResults.cc($rebinnedDetFile, $rebinnedDetFile2, \"hDet\")"
  fi

fi
