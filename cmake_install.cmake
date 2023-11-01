# Install script for directory: /dune/app/users/wenjiewu/FLArE_Dev/FLArE

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/FLArE" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/FLArE")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/FLArE"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/dune/app/users/wenjiewu/FLArE_Dev/FLArE/FLArE")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/FLArE" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/FLArE")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/FLArE"
         OLD_RPATH "/cvmfs/larsoft.opensciencegrid.org/products/root/v6_22_06a/Linux64bit+3.10-2.17-e20-p383b-prof/lib:/cvmfs/larsoft.opensciencegrid.org/products/genie/v3_00_06k/Linux64bit+3.10-2.17-e20-prof/lib:/cvmfs/larsoft.opensciencegrid.org/products/log4cpp/v1_1_3c/Linux64bit+3.10-2.17-e20-prof/lib:/cvmfs/larsoft.opensciencegrid.org/products/pythia/v6_4_28r/Linux64bit+3.10-2.17-gcc930-prof/lib:/cvmfs/larsoft.opensciencegrid.org/products/hep_hpc/v0_14_01/Linux64bit+3.10-2.17-e20-prof/lib:/cvmfs/larsoft.opensciencegrid.org/products/geant4/v4_10_6_p01c/Linux64bit+3.10-2.17-e20-prof/lib64:/cvmfs/larsoft.opensciencegrid.org/products/gsl/v2_6a/Linux64bit+3.10-2.17/lib:/cvmfs/larsoft.opensciencegrid.org/products/libxml2/v2_9_10a/Linux64bit+3.10-2.17/lib:/cvmfs/larsoft.opensciencegrid.org/products/hdf5/v1_12_0b/Linux64bit+3.10-2.17-e20-prof/lib:/cvmfs/larsoft.opensciencegrid.org/products/xerces_c/v3_2_3a/Linux64bit+3.10-2.17-e20/lib:/cvmfs/larsoft.opensciencegrid.org/products/clhep/v2_4_1_3a/Linux64bit+3.10-2.17-e20-prof/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/FLArE")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/dune/app/users/wenjiewu/FLArE_Dev/FLArE/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
