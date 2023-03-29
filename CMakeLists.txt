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
#if(WITH_ANALYSIS_USE)
#add_definitions(-DUSEROOT)
#else()
#endif()

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT REQUIRED COMPONENTS Geom EG EGPythia6)
include(${ROOT_USE_FILE})
#include_directories(${ROOT_INCLUDE_DIRS})
message(STATUS "Set ROOT : ${ROOT_USE_FILE}")
#if(WITH_ANALYSIS_USE)
#	EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
#endif(WITH_ANALYSIS_USE) 
#message(STATUS "Set ROOT : ${ROOT_CXX_FLAGS}")

## GENIE
if (GENIE STREQUAL "") 
  cmessage(FATAL_ERROR "Variable GENIE is not defined.")
endif()
execute_process (COMMAND genie-config --topsrcdir OUTPUT_VARIABLE GENIE_INC_DIR)
execute_process (COMMAND genie-config --libs OUTPUT_VARIABLE GENIE_LIBS_DIR_WITH_LFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process (COMMAND genie-config --libdir OUTPUT_VARIABLE GENIE_LIBS_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
include_directories(${GENIE_INC_DIR})
link_directories(${GENIE_LIBS_DIR})
message(STATUS "GENIE inc : ${GENIE_INC_DIR}")
message(STATUS "GENIE libdir  : ${GENIE_LIBS_DIR}")
message(STATUS "GENIE libs    : ${GENIE_LIBS_DIR_WITH_LFLAGS}")
set(GENIE_LIBRARIES "${GENIE_LIBS_DIR}/libGFwAlg.so;${GENIE_LIBS_DIR}/libGFwEG.so;${GENIE_LIBS_DIR}/libGFwGHEP.so;${GENIE_LIBS_DIR}/libGFwInt.so;${GENIE_LIBS_DIR}/libGFwMsg.so;${GENIE_LIBS_DIR}/libGFwNtp.so;${GENIE_LIBS_DIR}/libGFwNum.so;${GENIE_LIBS_DIR}/libGFwParDat.so;${GENIE_LIBS_DIR}/libGFwReg.so;${GENIE_LIBS_DIR}/libGFwUtl.so;${GENIE_LIBS_DIR}/libGPhAMNGEG.so;${GENIE_LIBS_DIR}/libGPhAMNGXS.so;${GENIE_LIBS_DIR}/libGPhBDMEG.so;${GENIE_LIBS_DIR}/libGPhBDMXS.so;${GENIE_LIBS_DIR}/libGPhChmXS.so;${GENIE_LIBS_DIR}/libGPhCmn.so;${GENIE_LIBS_DIR}/libGPhCohEG.so;${GENIE_LIBS_DIR}/libGPhCohXS.so;${GENIE_LIBS_DIR}/libGPhDcy.so;${GENIE_LIBS_DIR}/libGPhDeEx.so;${GENIE_LIBS_DIR}/libGPhDfrcEG.so;${GENIE_LIBS_DIR}/libGPhDfrcXS.so;${GENIE_LIBS_DIR}/libGPhDISEG.so;${GENIE_LIBS_DIR}/libGPhDISXS.so;${GENIE_LIBS_DIR}/libGPhGlwResEG.so;${GENIE_LIBS_DIR}/libGPhGlwResXS.so;${GENIE_LIBS_DIR}/libGPhHadnz.so;${GENIE_LIBS_DIR}/libGPhHadTransp.so;${GENIE_LIBS_DIR}/libGPhIBDEG.so;${GENIE_LIBS_DIR}/libGPhIBDXS.so;${GENIE_LIBS_DIR}/libGPhMEL.so;${GENIE_LIBS_DIR}/libGPhMNucEG.so;${GENIE_LIBS_DIR}/libGPhMNucXS.so;${GENIE_LIBS_DIR}/libGPhNDcy.so;${GENIE_LIBS_DIR}/libGPhNNBarOsc.so;${GENIE_LIBS_DIR}/libGPhNuclSt.so;${GENIE_LIBS_DIR}/libGPhNuElEG.so;${GENIE_LIBS_DIR}/libGPhNuElXS.so;${GENIE_LIBS_DIR}/libGPhPDF.so;${GENIE_LIBS_DIR}/libGPhQELEG.so;${GENIE_LIBS_DIR}/libGPhQELXS.so;${GENIE_LIBS_DIR}/libGPhResEG.so;${GENIE_LIBS_DIR}/libGPhResXS.so;${GENIE_LIBS_DIR}/libGPhStrEG.so;${GENIE_LIBS_DIR}/libGPhStrXS.so;${GENIE_LIBS_DIR}/libGPhXSIg.so;${GENIE_LIBS_DIR}/libGRwClc.so;${GENIE_LIBS_DIR}/libGRwFwk.so;${GENIE_LIBS_DIR}/libGRwIO.so;${GENIE_LIBS_DIR}/libGTlFlx.so;${GENIE_LIBS_DIR}/libGTlGeo.so")

## log4cpp
find_library(log4cpp NAMES log4cpp PATHS $ENV{LOG4CPP_LIB} PATH_SUFFIXES lib lib64 NO_DEFAULT_PATH)
include_directories("$ENV{LOG4CPP_INC}")
link_directories("$ENV{LOG4CPP_LIB}")
message(STATUS "log4cpp inc : $ENV{LOG4CPP_INC}")
set(LOG4CPP_LIBRARIES "$ENV{LOG4CPP_LIB}/liblog4cpp.so")

## gsl
find_package(GSL REQUIRED)

## libxml2
find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})
message(STATUS "libxml2 inc : ${LIBXML2_INCLUDE_DIR}")

## pythia6
include_directories($ENV{PYTHIA_INC})
link_directories("$ENV{PYTHIA6_LIBRARY}")
message(STATUS "pythia6 inc : $ENV{PYTHIA_INC}")
set(PYTHIA_LIRARIES "$ENV{PYTHIA6_LIBRARY}/libPythia6.so")

## Eigen
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include(${EIGEN3_USE_FILE})
message(STATUS "Eigen3 : ${EIGEN3_USE_FILE}")

# Locate sources and headers for this project
# We presume the existence of three directories
include_directories(${ROOT_INCLUDE_DIR} ${Geant4_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include)
#include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/src/reco/*.cc)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.hh 
  ${PROJECT_SOURCE_DIR}/include/reco/*.hh
  ${PROJECT_SOURCE_DIR}/include/utils/*.hh)
file(GLOB_RECURSE macros RELATIVE ${PROJECT_SOURCE_DIR} macros/*.mac)
file(GLOB_RECURSE genie RELATIVE ${PROJECT_SOURCE_DIR} genie/*)
file(GLOB_RECURSE analysis RELATIVE ${PROJECT_SOURCE_DIR} analysis/*)

# Enable macros for out-of-source build
foreach(_file ${macros} ${genie} ${analysis} ${GridUtils})
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
#if(WITH_ANALYSIS_USE)
#	EXECUTE_PROCESS(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
#	set(CMAKE_EXE_LINKER_FLAGS "${ROOT_LD_FLAGS} -lGeom")
#endif(WITH_ANALYSIS_USE)
#message(STATUS "ROOT : ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "ROOT : ${ROOT_LIBRARIES}")
target_link_libraries(FLArE 
  ${ROOT_LIBRARIES} 
  ${Geant4_LIBRARIES} 
  GSL::gsl GSL::gslcblas 
  ${LOG4CPP_LIBRARIES}
  ${LIBXML2_LIBRARIES}
  ${PYTHIA_LIRARIES}
  ${GENIE_LIBRARIES}
  )

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS FLArE DESTINATION bin)
