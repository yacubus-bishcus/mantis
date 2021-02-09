#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(mantis)
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON) # Can switch to OFF if not building with vis drivers --> NOT recommended 
add_compile_options(-Wno-unused-variable -Wno-shadow -Wno-unused-parameter)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 10.5 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 10.5 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find ROOT (required package)

find_package(ROOT 18.0 CONFIG REQUIRED)
if(ROOT_FOUND)	
    message(STATUS "ROOT found. --> Compiling with ROOT enabled")	
else()	
    message(STATUS "G4 ROOT NOT Found. --> MAKE WILL FAIL!")
endif()

#----------------------------------------------------------------------------
# Locate Sources and Headers 
include("${ROOT_USE_FILE}")
include_directories(${PROJECT_SOURCE_DIR}/include	
                    ${Geant4_INCLUDE_DIR}	                   
                    ${ROOT_INCLUDE_DIR})	
                    
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)	
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 and ROOT libraries
#
add_executable(mantis mantis.cc ${sources} ${headers})
target_link_libraries(mantis ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build mantis. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(Input_SCRIPTS
  mantis.in
  vis_save.mac
  )

set(Run_Analysis_SCRIPTS
PrintResults.cc
stitch.sh
)

set(Slurm_Run_Files_SCRIPTS
runBatch.sh
submit_geant4.slurm
)

foreach(_script ${Input_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/Input_Files/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()
foreach(_script ${Run_Analysis_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/Run_Analysis/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()
foreach(_script ${Slurm_Run_Files_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/Slurm_Run_Files/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS mantis DESTINATION bin)

# Pass version on
geant4_version = (geant4-config --version)
echo $geant4_version
