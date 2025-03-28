#!/bin/bash

file_list=$1 # one text file contains location of all the run configuration files
geofile=$2   # path to configuration file containing the geometry
macro=$3     # executive file of the main program (here the geant4 program)
out_dir=$4   # output directory

if [ -d "${out_dir}" ]; then
  if [ "$(ls -A ${out_dir})" ]; then
    echo "Take action ${out_dir} is not empty"
    return 1
  else
    echo "${out_dir} is empty"
    echo "submitting job(s)"
  fi
else
  echo "${out_dir} is not found"
  return 1
fi

n_jobs=`wc -l $file_list | cut -f1 -d' '`
jobsub_submit -N $n_jobs -G dune --expected-lifetime=10h --memory=40000MB --disk=8GB --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC,OFFSITE file://./batch_script.sh $file_list $geofile $macro $out_dir
