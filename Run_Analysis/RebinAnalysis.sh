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
TTEST="false"
VERBOSITY="false"

current_dir="$(pwd)"
file_var="/Rebin.cc"
file_var2="/PrintResults.cc"
FILECHECK=$current_dir$file_var
PRINTFILECHECK=$current_dir$file_var2
if test -e "$FILECHECK"; then
  echo "$FILECHECK found."
else
  echo "$FILECHECK not found."
  exit 1
fi

for arg in "$@"
do
   case $arg in
        -h|--help)
        echo "Rebinning Script for Mantis Output"
        echo "options:"
        echo "-h,--help               show brief help"
        echo "--fOn                   specify the on file to be read in to rebin"
        echo "--fOff=NA               specify the off file to be read"
        echo "--ChopData=false        specify chopper data analysis"
        echo "--IntObjData=false      specify Interrogation Object analysis"
        echo "--NRFData=false         specify NRF Data Analysis"
        echo "--WaterData=false       specify Water Data Analysis"
        echo "--CherenkovData=false   specify Cherenkov Data Analysis"
        echo "--DetData=true          specify Detected Data Analysis"
        echo "--IncDetData=false      specify Incident Detector Data Analysis"
        echo "--PrintData=false       Run PrintResults.cc"
        echo "--PrintDataOnly=false   Only Run Print Results.cc Do not Rebin!"
        echo "--TTest=false           Only Create Histograms for testing Chop Off thickness"
        echo "--Verbosity=false       specify data verbosity"
        shift
        exit 0
        ;;
        -a|--fOn=*)
        INFILE="${arg#*=}"
        shift
        ;;
        -b|--fOff=*)
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
       -T|--TTest=*)
       TTEST="${arg#*=}"
       shift
       ;;
       -Verbose|--Verbosity=*)
       VERBOSITY="${arg#*=}"
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
  echo "TTEST: $TTEST"
  echo "Verbosity: $VERBOSITY"

  if [ $PRINTDATAONLY == "false" ]
  then
    echo "Rebinning File: $INFILE"
  fi
  if [ $PRINTDATA == "true" ]
  then
    if test -e "$PRINTFILECHECK"; then
      echo "$PRINTFILECHECK found."
    else
      echo "$PRINTFILECHECK not found."
      exit 1
    fi
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

if [ $TTEST != "false" ]
then
  theFile=$VAR1$INFILE$VAR1
  theFile2=$VAR1$INFILE2$VAR1
  root -l -b -q "Rebin.cc($theFile,\"ChopOut\",\"hChopOut\")"
  root -l -b -q "Rebin.cc($theFile2,\"ChopOut\",\"hChopOut\")"
  root -l -b -q "Rebin.cc($theFile,\"IntObjIn\",\"hIntObjIn\")"
  root -l -b -q "Rebin.cc($theFile2,\"IntObjIn\",\"hIntObjIn\")"
  root -l -b -q "Rebin.cc($theFile,\"IntObjOut\",\"hIntObjOut\")"
  root -l -b -q "Rebin.cc($theFile2,\"IntObjOut\",\"hIntObjOut\")"
  root -l -b -q "Rebin.cc($theFile, \"DetInfo\",\"hDet\")"
  root -l -b -q "Rebin.cc($theFile2, \"DetInfo\",\"hDet\")"

  VAR2="converted_"
  rebinnedFile=$VAR1$VAR2$INFILE$VAR1
  rebinnedFile2=$VAR1$VAR2$INFILE2$VAR1

  root -l -b -q "PrintResults.cc($rebinnedFile, $rebinnedFile2, \"hChopOut\")"
  root -l -b -q "PrintResults.cc($rebinnedFile, $rebinnedFile2, \"hIntObjIn\")"
  root -l -b -q "PrintResults.cc($rebinnedFile, $rebinnedFile2, \"hIntObjOut\")"
  root -l -b -q "PrintResults.cc($rebinnedFile, $rebinnedFile2, \"hDet\")"

  exit 0
fi

if [ $PRINTDATAONLY == "false" ]
then

  theFile=$VAR1$INFILE$VAR1
  theFile2=$VAR1$INFILE2$VAR1

  if [ $CHOPDATA != "false" ] && [ $CHOPDATA != "False" ]
  then

    if [ $VERBOSITY != "false" ] && [ $VERBOSITY != "False" ]
    then
      root -l -b -q "Rebin.cc(true,$theFile,\"ChopIn\",\"hChopIn\",300)"
      root -l -b -q "Rebin.cc(true,$theFile,\"ChopOut\",\"hChopOut\",300)"
    else
      root -l -b -q "Rebin.cc(false,$theFile,\"ChopIn\",\"hChopIn\",300)"
      root -l -b -q "Rebin.cc(false,$theFile,\"ChopOut\",\"hChopOut\",300)"
    fi

    if [ $INFILE2 != "NA" ]
    then
      if [ $VERBOSITY != "false" ] && [ $VERBOSITY != "False" ]
      then
        root -l -b -q "Rebin.cc(true,$theFile2,\"ChopIn\",\"hChopIn\",300)"
        root -l -b -q "Rebin.cc(true,$theFile2,\"ChopOut\",\"hChopOut\",300)"
      else
        root -l -b -q "Rebin.cc(false,$theFile2,\"ChopIn\",\"hChopIn\",300)"
        root -l -b -q "Rebin.cc(false,$theFile2,\"ChopOut\",\"hChopOut\",300)"
      fi
    fi
  fi

  if [ $INTOBJDATA != "false" ] && [ $INTOBJDATA != "False" ]
  then
    if [ $VERBOSITY != "false" ] && [ $VERBOSITY != "False" ]
    then
      # Rebin IntObj > 1.5 MeV with 600 bins bin width 1e-3
      root -l -b -q "Rebin.cc(true,$theFile,\"IntObjIn\",\"hIntObjIn\",600,1.5,2.1,\"Energy>1.5\")"
      # Rebin IntObj > 1.5 MeV with 120,000 bins bin width 5e-6
      root -l -b -q "Rebin.cc(true,$theFile,\"IntObjIn\",\"hIntObjIn\",120000,1.5,2.1,\"Energy>1.5\")"
      root -l -b -q "Rebin.cc(true,$theFile,\"IntObjIn\",\"hIntObjIn\",300)"
      root -l -b -q "Rebin.cc(true,$theFile,\"IntObjOut\",\"hIntObjOut\",300)"
    else
      # Rebin IntObj > 1.5 MeV with 600 bins bin width 1e-3
      root -l -b -q "Rebin.cc(false,$theFile,\"IntObjIn\",\"hIntObjIn\",600,1.5,2.1,\"Energy>1.5\")"
      # Rebin IntObj > 1.5 MeV with 120,000 bins bin width 5e-6
      root -l -b -q "Rebin.cc(false,$theFile,\"IntObjIn\",\"hIntObjIn\",120000,1.5,2.1,\"Energy>1.5\")"
      root -l -b -q "Rebin.cc(false,$theFile,\"IntObjIn\",\"hIntObjIn\",300)"
      root -l -b -q "Rebin.cc(false,$theFile,\"IntObjOut\",\"hIntObjOut\",300)"
    fi
    if [ $INFILE2 != "NA" ]
    then
      if [ $VERBOSITY != "false" ] && [ $VERBOSITY != "False" ]
      then
        # Rebin IntObj > 1.5 MeV with 600 bins bin width 1e-3
        root -l -b -q "Rebin.cc(true,$theFile2,\"IntObjIn\",\"hIntObjIn\",600,1.5,2.1,\"Energy>1.5\")"
        # Rebin IntObj > 1.5 MeV with 120,000 bins bin width 5e-6
        root -l -b -q "Rebin.cc(true,$theFile2,\"IntObjIn\",\"hIntObjIn\",120000,1.5,2.1,\"Energy>1.5\")"
        root -l -b -q "Rebin.cc(true,$theFile2,\"IntObjIn\",\"hIntObjIn\",300)"
        root -l -b -q "Rebin.cc(true,$theFile2,\"IntObjOut\",\"hIntObjOut\",300)"
      else
        # Rebin IntObj > 1.5 MeV with 600 bins bin width 1e-3
        root -l -b -q "Rebin.cc(false,$theFile2,\"IntObjIn\",\"hIntObjIn\",600,1.5,2.1,\"Energy>1.5\")"
        # Rebin IntObj > 1.5 MeV with 120,000 bins bin width 5e-6
        root -l -b -q "Rebin.cc(false,$theFile2,\"IntObjIn\",\"hIntObjIn\",120000,1.5,2.1,\"Energy>1.5\")"
        root -l -b -q "Rebin.cc(false,$theFile2,\"IntObjIn\",\"hIntObjIn\",300)"
        root -l -b -q "Rebin.cc(false,$theFile2,\"IntObjOut\",\"hIntObjOut\",300)"
      fi
    fi
  fi

  if [ $NRFDATA != "false" ] && [ $NRFDATA != "False" ]
  then
    if [ $VERBOSITY != "false" ] && [ $VERBOSITY != "False" ]
    then
      root -l -b -q "Rebin.cc(true,$theFile,\"NRF\",\"hNRF\",300)"
    else
      root -l -b -q "Rebin.cc(false,$theFile,\"NRF\",\"hNRF\",300)"
    fi
    if [ $INFILE2 != "NA" ]
    then
      if [ $VERBOSITY != "false" ] && [ $VERBOSITY != "False" ]
      then
        root -l -b -q "Rebin.cc(true,$theFile2,\"NRF\",\"hNRF\",300)"
      else
        root -l -b -q "Rebin.cc(false,$theFile2,\"NRF\",\"hNRF\",300)"
      fi
    fi
  fi

  if [ $WATERDATA != "false" ] && [ $WATERDATA != "False" ]
  then
    if [ $VERBOSITY != "false" ] && [ $VERBOSITY != "False" ]
    then
      root -l -b -q "Rebin.cc(true,$theFile,\"Water\",\"hWater\",300)"
    else
      root -l -b -q "Rebin.cc(false,$theFile,\"Water\",\"hWater\",300)"
    fi
    if [ $INFILE2 != "NA" ]
    then
      if [ $VERBOSITY != "false" ] && [ $VERBOSITY != "False" ]
      then
        root -b -q "Rebin.cc(true,$theFile2,\"Water\",\"hWater\",300)"
      else
        root -b -q "Rebin.cc(false,$theFile2,\"Water\",\"hWater\",300)"
      fi
    fi
  fi

  if [ $CHERENKOVDATA != "false" ] && [ $CHERENKOVDATA != "False" ]
  then
    if [ $VERBOSITY != "false" ] && [ $VERBOSITY != "False" ]
    then
      root -l -b -q "Rebin.cc(true,$theFile,\"Cherenkov\",\"hCherenkov\",300)"
    else
      root -l -b -q "Rebin.cc(false,$theFile,\"Cherenkov\",\"hCherenkov\",300)"
    fi
    if [ $INFILE2 != "NA" ]
    then
      if [ $VERBOSITY != "false" ] && [ $VERBOSITY != "False" ]
      then
        root -l -b -q "Rebin.cc(true,$theFile2,\"Cherenkov\",\"hCherenkov\",300)"
      else
        root -l -b -q "Rebin.cc(false,$theFile2,\"Cherenkov\",\"hCherenkov\",300)"
      fi
    fi
  fi

  if [ $DETDATA != "false" ] && [ $DETDATA != "False" ]
  then
    if [ $VERBOSITY != "false" ] && [ $VERBOSITY != "False" ]
    then
      # Rebin Detected with 20 bins Energy < 5e-6 bin width 0.25e-6
      root -l -b -q "Rebin.cc(true,$theFile,\"DetInfo\",\"hDet\",20,0.0,5e-6,\"Energy<5e-6\")"
    else
      root -l -b -q "Rebin.cc(false,$theFile,\"DetInfo\",\"hDet\",20,0.0,5e-6,\"Energy<5e-6\")"
    fi
    if [ $INFILE2 != "NA" ]
    then
      if [ $VERBOSITY != "false" ] && [ $VERBOSITY != "False" ]
      then
        root -l -b -q "Rebin.cc(true,$theFile2,\"DetInfo\",\"hDet\",20,0.0,5e-6,\"Energy<5e-6\")"
      else
        root -l -b -q "Rebin.cc(false,$theFile2,\"DetInfo\",\"hDet\",20,0.0,5e-6,\"Energy<5e-6\")"
      fi
    fi
  fi

  if [ $INCDETDATA != "false" ] && [ $INCDETDATA != "False" ]
  then
    if [ $VERBOSITY != "false" ] && [ $VERBOSITY != "False" ]
    then
      root -l -b -q "Rebin.cc(true,$theFile,\"IncDetInfo\",\"hIncDetInfo\",300)"
    else
      root -l -b -q "Rebin.cc(false,$theFile,\"IncDetInfo\",\"hIncDetInfo\",300)"
    fi
    if [ $INFILE2 != "NA" ]
    then
      if [ $VERBOSITY != "false" ] && [ $VERBOSITY != "False" ]
      then
        root -l -b -q "Rebin.cc(true,$theFile2,\"IncDetInfo\",\"hIncDetInfo\",300)"
      else
        root -l -b -q "Rebin.cc(false,$theFile2,\"IncDetInfo\",\"hIncDetInfo\",300)"
      fi
    fi
  fi
fi


# PRINTING RESULTS (RUNNING Z TESTS)

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
