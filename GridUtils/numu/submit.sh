#!/bin/bash

file_list=$1 # one text file contains location of all the run configuration files
macro=$2     # executive file of the main program (here the geant4 program)
out_dir=$3   # output directory

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
jobsub_submit -N $n_jobs -G dune --expected-lifetime=8h --memory=24000MB --disk=8GB --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC file://./batch_script.sh $file_list $macro $out_dir
