#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
project(FLArE)

message(STATUS "Project Source Dir : ${PROJECT_SOURCE_DIR}")

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})
message(STATUS "Set Geant4 : ${Geant4_USE_FILE}")

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT REQUIRED COMPONENTS Geom EG RIO) 
include(${ROOT_USE_FILE})
message(STATUS "Set ROOT : ${ROOT_USE_FILE}")
message(STATUS "ROOT : ${ROOT_LIBRARIES}")

## Eigen
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include(${EIGEN3_USE_FILE})
message(STATUS "Eigen3 : ${EIGEN3_USE_FILE}")

## HDF5 --> this is not needed, find_package() called within HEP_HPC
#find_package(HDF5 REQUIRED) 
#include_directories(${HDF5_INCLUDE_DIRS})
#message(STATUS "HDF5 lib: ${HDF5_LIBRARIES}")
#message(STATUS "HDF5 inc: ${HDF5_INCLUDE_DIRS}")

## HEP_HPC
## needs a little nudge to fing it using env variable from setup
list(APPEND CMAKE_PREFIX_PATH $ENV{hep_hpc_path})
find_package(hep_hpc)
include_directories(${HEP_HPC_INCLUDE_DIRS})
message(STATUS "HEP_HPC inc: ${HEP_HPC_INCLUDE_DIRS}")
link_directories("$ENV{hep_hpc_path}/lib")
set(HEP_HPC_LIBRARIES "$ENV{hep_hpc_path}/lib/libhep_hpc_hdf5.so;$ENV{hep_hpc_path}/lib/libhep_hpc_Utilities.so;$ENV{hep_hpc_path}/lib/libhep_hpc_concat_hdf5.so")
message(STATUS "HEP_HPC lib: ${HEP_HPC_LIBRARIES}")


# Locate sources and headers for this project
# We presume the existence of three directories

include_directories(${ROOT_INCLUDE_DIR} ${Geant4_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cc 
                          ${PROJECT_SOURCE_DIR}/src/reco/*.cc
                          ${PROJECT_SOURCE_DIR}/src/geometry/*.cc
                          ${PROJECT_SOURCE_DIR}/src/fields/*.cc)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.hh 
                          ${PROJECT_SOURCE_DIR}/include/reco/*.hh
                          ${PROJECT_SOURCE_DIR}/include/utils/*.hh
                          ${PROJECT_SOURCE_DIR}/include/geometry/*.hh
                          ${PROJECT_SOURCE_DIR}/include/fields/*.hh)
file(GLOB_RECURSE macros RELATIVE ${PROJECT_SOURCE_DIR} macros/*.mac)
file(GLOB_RECURSE genie RELATIVE ${PROJECT_SOURCE_DIR} genie/*)
file(GLOB_RECURSE analysis RELATIVE ${PROJECT_SOURCE_DIR} analysis/*)
file(GLOB_RECURSE gridutils RELATIVE ${PROJECT_SOURCE_DIR} GridUtils/*)
file(GLOB setup RELATIVE ${PROJECT_SOURCE_DIR} lxplus_setup.sh)

# Enable macros for out-of-source build
foreach(_file ${macros} ${genie} ${analysis} ${gridutils} ${setup})
  configure_file(
    ${_file}
    ${PROJECT_BINARY_DIR}/${_file}
    COPYONLY
    )
    #message(STATUS "Copying ${_file} in ${PROJECT_BINARY_DIR}/${_file}")
endforeach()

#----------------------------------------------------------------------------
# generate dictionaries for the custom classes in the ROOT tree
ROOT_GENERATE_DICTIONARY(DictOutput ${PROJECT_SOURCE_DIR}/include/FPFNeutrino.hh 
                                    ${PROJECT_SOURCE_DIR}/include/FPFParticle.hh 
				    LINKDEF ${PROJECT_SOURCE_DIR}/include/LinkDef.h)

# create a shared library that includes the dictionary
add_library(FPFClasses SHARED ${PROJECT_SOURCE_DIR}/src/FPFNeutrino.cc
                              ${PROJECT_SOURCE_DIR}/src/FPFParticle.cc
                              DictOutput.cxx)

# include directories for headers
target_include_directories(FPFClasses PUBLIC ${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(FLArE FLArE.cc ${sources} ${headers})

target_link_libraries(FLArE 
  ${ROOT_LIBRARIES} 
  ${Geant4_LIBRARIES} 
  ${HDF5_LIBRARIES}
  ${HEP_HPC_LIBRARIES}
  FPFClasses
  )

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS FLArE DESTINATION ${PROJECT_SOURCE_DIR}/bin)
install(TARGETS FPFClasses DESTINATION ${PROJECT_SOURCE_DIR}/lib)
install(FILES ${PROJECT_SOURCE_DIR}/build/libDictOutput_rdict.pcm DESTINATION ${PROJECT_SOURCE_DIR}/lib)
