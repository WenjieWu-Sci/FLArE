#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(FLArE)

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

# Setup of ROOT Analysis : optional
option(WITH_ANALYSIS_USE "Build example with analysis objects" ON)
if(WITH_ANALYSIS_USE)
add_definitions(-DUSEROOT)
else()
endif()

#list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
#
#find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})
#include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT REQUIRED)
if(WITH_ANALYSIS_USE)
	EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
endif(WITH_ANALYSIS_USE) 

# Locate sources and headers for this project
# We presume the existence of three directories
include_directories(${ROOT_INCLUDE_DIR} ${Geant4_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB_RECURSE macros RELATIVE ${PROJECT_SOURCE_DIR} macros/*.mac)

# Enable macros for out-of-source build
foreach(_file ${macros})
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
if(WITH_ANALYSIS_USE)
	EXECUTE_PROCESS(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
	set(CMAKE_EXE_LINKER_FLAGS ${ROOT_LD_FLAGS})
endif(WITH_ANALYSIS_USE)
target_link_libraries(FLArE ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS FLArE DESTINATION bin)
