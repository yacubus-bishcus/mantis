# Mantis
This program allows the user to run calorimetric NRF Cherenkov Detector cargo screening simulations. This simulation provides visualization tools in addition to root histogramming data. Visualization tools can be utilized in both batch and interactive modes. The final output provided can be manipulated by the user through the mantis.in input. Available histograms include: Chopper Wheel Incident Data, Incident Energy Water Tank Data, Incident Energy Photocathode Data, Detected Energy Spectrum Data. Additionally, an ntuple consisting of Detector Processes Data is available upon user request. This simulation allows the user to track particles at each phase in the experimental set up while manipulating the geometries, materials, and Chopper On/Off State through the mantis.in macro. 

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

The Following Instructions should work once the Geant4 and CERN ROOT are installed:

To Build
==

Now go to geant4 application
`> mkdir <application_build> && cd <application_build>`
`> cmake /path/to/application/source && make -j[N]`

To Run in Batch Mode
==
vis_save.mac will make a .wrl visualization file based on the commands provided in vis_save.mac
mantis.in will not create a visualization. 
`> ./mantis -m macro(mantis.in or vis_save.mac) -o <root output filename> -s <seed>`
  
To Run in Interactive Mode
==
The following will use OPENGL in interactive mode 
`> ./mantis`
  
Required Inputs
==

Mantis Requires one input:

* brems_distributions.root - This is the input spectrum file that is read as long as the input/energy is less than 0. Reading from the input spectrum file is set as the default. The bremstrahlung input and sampling distribution can be easily manipulated with Sampling.cc  

Output
==

The Code will generate a .log and _error.log file in addition to a root file. All G4cout is sent to .log and all G4cerr is sent to _error.log. The Root File will consist of 4 histograms and 1 TTree. The user must select output in mantis.in for the histograms/TTree to be filled during the simulation. 
