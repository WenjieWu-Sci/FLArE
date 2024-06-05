#!/bin/bash

cluster=$1
process=$2

# source the environment
source lxplus_setup.sh

# create log directories
mkdir -p /afs/cern.ch/work/m/mvicenzi/public/LOGS/${cluster}/out
mkdir -p /afs/cern.ch/work/m/mvicenzi/public/LOGS/${cluster}/err
mkdir -p /afs/cern.ch/work/m/mvicenzi/public/LOGS/${cluster}/log

# import gst file if needed by mac file
eos cp /eos/user/m/mvicenzi/genie/numu_kling_ar40_e5000.gst.root . 

echo "Running ./FLArE macro.mac"
./FLArE macro.mac
