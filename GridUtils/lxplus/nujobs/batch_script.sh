#!/bin/bash

cluster=$1
process=$2
gstfile=$3

echo "Executing JOBID ${cluster}.${process}"

# source the environment
source lxplus_setup.sh

# import gst file
gst=`basename "${gstfile}"`
echo "Importing ${gst}..."
eos cp ${gstfile} .

# select the macro file
let num=$((${process}+1))
echo "Selecting macro from line ${num} in list:"

macropath=$(tail -n+${num} macrolist.txt | head -n1)
macro=`basename "$macropath"`
echo "$macro"

# import the macro file
cp ${macropath} .

# running !!
echo "Running ./FLArE ${macro}"
./FLArE ${macro}

# delete what you imported, or it is copied in output!
rm ${macro} ${gst}

echo "Completed JOBID ${cluster}.${process}"
