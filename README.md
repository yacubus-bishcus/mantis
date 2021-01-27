# Mantis
This program allows the user to run calorimetric NRF Cherenkov Detector cargo screening simulations. This simulation provides visualization tools in addition to root histogramming data. Visualization tools can be utilized in both batch and interactive modes. The final output provided can be manipulated by the user through the "mantis.in" input. 

Mantis Output
==

Eight(8) histograms are available upon user request: 

1. Weighted Interrogation Object Incident Data -> /output/myoutput IntObjData must be uncommented!
2. Weighted NRF Photons Incident Interrogation Object -> /output/myouput NRFData must be uncommented!
3. Weighted NRF Photons Incident Water Tank Data -> /output/myouput NRFData must be uncommented!
4. Weighted Low Energy Incident Water Tank Data -> /output/myoutput WaterIncidentData must be uncommented!
 - Weighted energy spectrum of photons incident Water Tank < 1E-5 MeV
5. Weighted High Energy Incident Water Tank Data -> /output/myoutput WaterIncidentData must be uncommented!
 - Weighted energy spectrum of photons incident Water Tank > 1E-5 MeV
6. Weighted Low Energy Incident Photocathode Data -> /output/myoutput DetData must be uncommented!
 - Weighted energy spectrum of photons incident photocathode < 1E-5 MeV
7. Weighted High Energy Incident Photocathode Data -> /output/myoutput DetData must be uncommented!
 - Weighted energy spectrum of photons incident photocathode > 1E-5 MeV
8. Weighted Detected Energy Data -> /output/myoutput DetData must be uncommented!
 - Weighted energy spectrum of photons "detected" on photocathode includes Quantum efficiency!

Additionally, four(4) ntuples(TTrees) are available upon user request:

1. Incident Chopper Energy Data -> /output/myoutput ChopperData must be uncommented!
 - Unweighted Incident Chopper Energies 

2. Cherenkov Data -> /cherenkov/myoutput CherenkovData must be uncommented!
 - Event Beam Energy (unweighted)
 - Number of Photons Emitted in Event
 
3. NRF Material Data -> /output/myouput/NRFData must be uncommented!
 - Particle NRF Energy (unweighted)
 - Name of Creation Physical Volume (chopper, IntObj, etc...)

4. Detector Process at PC Data -> /output/myoutput DetData must be uncommented!
 - Process that occurs at PC surface (absorbed, detected, reflected, etc.)

Mantis Input
==

The User can manipulate the simulation in various ways through the mantis.in macro input file. For example the following line of code:
`> ./mantis -m mantis.in -o test.root -s 1`
would run mantis with the inputs found in mantis.in located in the mantis executable directory. The ouput would be found in test.root and would have a seed of 1.

The command line input options and flags include:
`-m macro file`

`-n addNRF` -> IF set to false NRF Physics will be removed from physicsList! The default is set to true.

`-o output filename`

`-p print standalone.dat file` -> Calls G4NRF to print a file of NRF Energies (takes up to 15 min) not recommended for non-developers
`-s seed` 
`-t testBrem -> For creating a bremsstrahlung beam to run Sampling.cc on for importance sampling input distribution`
`-v NRF Verbose` 

__Mandatory Inputs for mantis.in__

mantis.in has the following MANDATORY inputs that the user must not comment:

1. Chopper State (On/Off)
2. Chopper Material (Uranium/Plutonium)
3. Chopper Material Isotopic Composition 
4. Interrogation Object Material
5. Interrogation Object Material Isotopic Composition if Interrogation Object is Uranium/Plutonium

The user also has several optional commands available in mantis.in:

1. Input Energy/Spectrum
2. Chopper Thickness
3. Chopper Distance from Source
4. Interrogation Object Location 
5. Interrogation Object Size (radius)
6. Detector Location
7. Attenuating Layers (state, material, thickness)
8. Photocathode (Size, number, material)
9. Output Desired 
10. Number of MC Particles to Simulate 

Author: Jacob E Bickus

Creation time: 8/2020 

Last Update: continous

To Install
==

Dependencies
  * Installed Geant4 Libraries 
  * CERN ROOT Installed 
  * Cmake 
  
__Version__:  Mantis and been built against and tested with Geant4 10.3 and 10.5, ROOT 6.14 and ROOT 6.16. Geant4 10.6 has important modifications that require changes in the code.

EASY SETUP WITH CONFIGURE FILE 
==

For Easy Set up run the following line of code:
`> git clone https://github.com/jacobbickus/mantis.git && cd mantis && chmod u+x configure.sh && ./configure.sh`

The terminal will ask first for the path to the CERN ROOT Directory where the user built ROOT. Then the Terminal will ask for the Geant4 install directory where the user has geant4 libraries installed. Once the user inputs these variables the configure file will build mantis and execute a test run with outputs: test_error.log, test.log, and test.root. 

INSTALLATION WITHOUT CONFIGURE FILE 
==

The Following Instructions should work once the Geant4 and CERN ROOT are installed:

Be sure to untar the NRF Database using 
`> tar xfz NRF_Database.tar.gz`

Export the path to the untarred NRF Database for example in the user's bash:

`export G4NRFGAMMADATA=/path/to/Database/Database1.1`

Also be sure to export the ROOT Cmake path in the directory where the user built ROOT for example in user's bash:

`export ROOTCMAKEPATH=/path/to/root_build_directory/etc/cmake`

Lastly some path issues may occur without the following lines in the user's bash:

`source /path/to/root_build_directory/bin/thisroot.sh`

`source /path/to/geant4-install/bin/geant4.sh`

`source /path/to/geant4-install/share/Geant4-10.5.1/geant4make/geant4make.sh`


To Build
==

Now go to geant4 application
`> mkdir <application_build> && cd <application_build>`

`> cmake /path/to/application/source && make -j[N]`

To Run in Batch Mode
==
"vis_save.mac" will make a .wrl visualization file based on the commands provided in "vis_save.mac".

"mantis.in" will not create a visualization. 
`> ./mantis -m macro(mantis.in or vis_save.mac) -o <root output filename> -s <seed>`
  
To Run in Interactive Mode
==
The following will use OPENGL in interactive mode 
`> ./mantis`
  
Required Inputs
==

Mantis Requires one input:

* brems_distributions.root - This is the input spectrum file that is read if the user does not uncomment the /input/energy line in mantis.in. The bremstrahlung input and sampling distribution can be easily manipulated with Sampling.cc  

Manipulating the Input Spectrum "brems_distributions.root" with Sampling.cc
==

Sampling.cc can be run with the following terminal command:

`> root -b -q 'Sampling.cc("brem.root",maxEnergyOfBremBeam)' `

Where the "brem.root" file is the bremsstrahulung spectrum expected to impact the chopper wheel. The "brem.root" file can be obtained with the instructions under the section "Creating a Bremsstrahlung Input Beam" below. The "maxEnergyOfBremBeam" is the same maximum energy of the input bremsstrahulung beam from the "brem.root" file. 

Sampling.cc creates two output files.

1. brems_distributions.root
2. brems_distributions.png 

The root file consists of two histograms: hBrems and hSample. These histograms are used to determine the importance sampling in the mantis program. The .png file is a depiction of the hBrems and hSampled overlayed on the same canvas. 

Creating a Bremsstrahlung Input Beam
==

The bremsstrahlung beam can be determined via other Geant4 simulations or through mantis. To create the input bremsstrahlung beam expected to inpinge upon the chopper wheel run mantis the following way:

`> ./mantis -m mantis.in -o brem.root -s <seed> -t true `

Adding the -t flag tells mantis that it should only output the Chopper Data TTree. Be sure to edit the following line in "mantis.in": 

`/input/energy maxEnergyOfBremBeam`


Output
==

The Code will generate a .log and _error.log file in addition to a root file. All G4cout is sent to .log and all G4cerr is sent to _error.log. The user must select output in "mantis.in" for the histograms/TTree to be filled during the simulation. 

Analysis
==

Several helper scripts are included:
1. runBatch.sh
* The main call for running in batch mode on PSFC engaging cluster through slurm
* Takes input <start Seed> <Last Seed> <Output FileName Root> where the number of jobs submitted = <Last Seed> - <start Seed>
 
2. submit_geant4.slurm
* This is the file for the slurm scheduler. Adjusting the time limit on each run can be important for scheduling 

3. stitch.sh 
* This merges the output files.
* Takes inputs <"Output_FileName_Root*.root"> <merged_FileName.root> 

4. PostAnalysis.cc 
* This allows for quick comparison of the On/Off chopper states of merged root files 
The PostAnalysis.cc file can be run in CERN ROOT with the command:

`> root -b -q 'PostAnalysis.cc("ChopperOn.root","ChopperOff.root")'`

The File outputs the On/Off States respective: Detected Entries, Detected Weighted Sum, NRF Incident Water Weighted Sum. It also outputs the percent difference between the Chopper On/Off detected weighted sums. 
