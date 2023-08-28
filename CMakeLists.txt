#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.0...3.5 FATAL_ERROR)
project(mantis)
option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON) # Can switch to OFF if not building with vis drivers --> NOT recommended 
option(WITH_DATA "Build with Database" OFF) # Required for first build

# Determine the Geant4 Version 
execute_process(COMMAND bash -c "geant4-config --version"
                OUTPUT_VARIABLE MY_GEANT4_VERSION
                )
# Determine the ROOT CERN Version 
execute_process(COMMAND bash -c "root --version"
                OUTPUT_VARIABLE MY_ROOT_VERSION
                )
                
string(SUBSTRING "${MY_GEANT4_VERSION}" 0 4 THE_VERSION)	
string(COMPARE EQUAL "${THE_VERSION}" "10.7" VERSION_BOOL)	

# If the version is 10.7 then add the compile definition to avoid depreceated code!
if(${VERSION_BOOL})	
  add_compile_definitions(G4_OPTPARAM)
  message(STATUS "Version is at least 10.7")
endif()

if(${MYVERBOSE})
  message(STATUS "Running make with Full Verbosity")
else()
  message(STATUS "Subduing some verbosity.")
  add_compile_options(-Wno-unused-variable -Wno-shadow -Wno-unused-parameter)
endif()

if(WITH_GEANT4_UIVIS)
  find_package(Geant4 10.5...<10.8 REQUIRED ui_all vis_all)
  message(STATUS "Geant4 Version: ${MY_GEANT4_VERSION}")
  message(STATUS "Built Geant4 with UI and Vis Drivers")
else()
  find_package(Geant4 10.5...<10.8 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find ROOT (required package)

find_package(ROOT 6.18...<6.24 CONFIG REQUIRED)
if(ROOT_FOUND)	
    message(STATUS "${MY_ROOT_VERSION}")	
else()	
    message(STATUS "ROOT NOT Found. --> MAKE WILL FAIL!")
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
add_executable(mantis mantis.cc ${sources} ${headers})
target_link_libraries(mantis ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

# Unzip and Move Database 
if(WITH_DATA)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${PROJECT_SOURCE_DIR}/NRF_Database.tar.gz
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
  message(STATUS "NRF Database Unpacked in ${PROJECT_SOURCE_DIR}!")
else()
  message(STATUS "NRF Database will remain Zipped")
endif()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
# Install Input and Run Files into Appropriate Directories
install(TARGETS mantis DESTINATION bin)
install(DIRECTORY Input_Files DESTINATION Default_Input)
install(DIRECTORY Run_Analysis DESTINATION Default_Run_Analysis)
if(WITH_DATA)
  install(DIRECTORY Database1.1 DESTINATION DATA)
  message(STATUS "NRF Database moved to ${CMAKE_INSTALL_PREFIX}")
endif()