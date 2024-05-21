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


# Setup of ROOT Analysis : optional
option(WITH_ANALYSIS_USE "Build example with analysis objects" ON)

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT REQUIRED COMPONENTS Geom EG EGPythia6 RIO)
include(${ROOT_USE_FILE})
#include_directories(${ROOT_INCLUDE_DIRS})
message(STATUS "Set ROOT : ${ROOT_USE_FILE}")

## Eigen
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include(${EIGEN3_USE_FILE})
message(STATUS "Eigen3 : ${EIGEN3_USE_FILE}")

## hep_hpc and hdf5
find_package(hdf5 REQUIRED EXPORT) 
include_directories($ENV{HDF5_INC})
set(HDF5_LIBRARIES "$ENV{HDF5_LIB}/libhdf5.so")
message(STATUS "HDF5 inc: $ENV{HDF5_INC}")
include_directories($ENV{HEP_HPC_INC})
link_directories("$ENV{HEP_HPC_LIB}")
set(HEP_HPC_LIBRARIES "$ENV{HEP_HPC_LIB}/libhep_hpc_hdf5.so;$ENV{HEP_HPC_LIB}/libhep_hpc_Utilities.so;$ENV{HEP_HPC_LIB}/libhep_hpc_concat_hdf5.so")
message(STATUS "hep_hpc inc: $ENV{HEP_HPC_INC}")

# Locate sources and headers for this project
# We presume the existence of three directories
include_directories(${ROOT_INCLUDE_DIR} ${Geant4_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include)
#include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cc 
                          ${PROJECT_SOURCE_DIR}/src/reco/*.cc
                          ${PROJECT_SOURCE_DIR}/src/geometry/*.cc)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.hh 
                          ${PROJECT_SOURCE_DIR}/include/reco/*.hh
                          ${PROJECT_SOURCE_DIR}/include/utils/*.hh
                          ${PROJECT_SOURCE_DIR}/include/geometry/*.hh)
file(GLOB_RECURSE macros RELATIVE ${PROJECT_SOURCE_DIR} macros/*.mac)
file(GLOB_RECURSE genie RELATIVE ${PROJECT_SOURCE_DIR} genie/*)
file(GLOB_RECURSE analysis RELATIVE ${PROJECT_SOURCE_DIR} analysis/*)
file(GLOB_RECURSE gridutils RELATIVE ${PROJECT_SOURCE_DIR} GridUtils/*)

# Enable macros for out-of-source build
foreach(_file ${macros} ${genie} ${analysis} ${gridutils})
  configure_file(
    ${_file}
    ${PROJECT_BINARY_DIR}/${_file}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(FLArE FLArE.cc ${sources} ${headers})
#message(STATUS "ROOT : ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "ROOT : ${ROOT_LIBRARIES}")
target_link_libraries(FLArE 
  ${ROOT_LIBRARIES} 
  ${Geant4_LIBRARIES} 
  ${HDF5_LIBRARIES}
  ${HEP_HPC_LIBRARIES}
  )

# attach dictionaries to the executable. first, tell it where to look for headers required by the dictionaries:
target_include_directories(FLArE PRIVATE ${PROJECT_SOURCE_DIR}/include)
# then generate dictionaries and add them as a dependency of the executable (via the MODULE parameter):
ROOT_GENERATE_DICTIONARY(DictOutput ${PROJECT_SOURCE_DIR}/include/FPFNeutrino.hh 
                                    ${PROJECT_SOURCE_DIR}/include/FPFParticle.hh 
                                    MODULE FLArE LINKDEF ${PROJECT_SOURCE_DIR}/include/LinkDef.h)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS FLArE DESTINATION bin)
