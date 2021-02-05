#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(mantis)
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
add_compile_options(-Wno-unused-variable -Wno-shadow -Wno-unused-parameter)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find ROOT (required package)
list(APPEND CMAKE_CURRENT_SOURCE_DIR $ENV{ROOTCMAKEPATH})
find_package(ROOT REQUIRED COMPONENTS RIO Net)
if(ROOT_FOUND)	
    message(STATUS "ROOT found. --> Compiling with ROOT enabled")	
else()	
    message(STATUS "G4 ROOT NOT Found. --> MAKE WILL FAIL!")
endif()

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
ROOT_GENERATE_DICTIONARY(G__mantis mantis.h LINKDEF mantisLinkDef.h)

# Create a shared library with a generated dictionary.
# Passing cmake targets such as `ROOT::RIO` as dependencies (rather than plain
# library names e.g. via ${ROOT_LIBRARIES}) ensures that properties such as required
# include directories and C++ standard are propagated to our libraries or executables.
# Note: to ensure compatibility with Cling, targets *must* be compiled using the
# same C++ standard as ROOT was compiled with.
add_library(mantis SHARED mantis.cc G__mantis.cc)
target_link_libraries(mantis PUBLIC ROOT::RIO ROOT::Net)
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(mantis mantis.cc ${sources} ${headers})
#target_link_libraries(mantis ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(mantis ${Geant4_LIBRARIES})

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
