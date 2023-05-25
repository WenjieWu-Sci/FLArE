#!/bin/bash

function gen_single_macro {
  field=$1; shift
  num_evt_per_file=$1; shift
  num_file=$1; shift
  loc_list=$1; shift  
  macro=$1; shift
  
  for i in `seq 1 1 ${num_file}`
  do
    seed1=$((42+i))
    seed2=$((47+i))

    istart=$((num_evt_per_file*i-num_evt_per_file))
    filename="/pnfs/dune/scratch/users/mvicenzi/job_sub/macros/numu/FLArE_numu_B_${field}T_${i}.mac"
    outputfile="FLArE_numu_B_${field}T_${i}.root"
    if [ -f ${filename} ]; then
      echo "${filename} exists, delete file first"
      ifdh rm ${filename}
      #rm ${filename}
    fi
    echo $filename >> $loc_list
    rm temp.mac
cat << EOF >> temp.mac
/det/material LAr
/det/saveGdml false
/det/field ${field} tesla

/random/setSeeds ${seed1} ${seed2}
/run/initialize

/genie/useGenie true
/genie/genieInput numu_kling_ar40_e5000.ghep.root
/genie/genieIStart ${istart}

/histo/saveEvd false
/histo/saveHit false
/histo/circleFit true
/histo/fileName ${outputfile}

/run/beamOn ${num_evt_per_file}
EOF
 
  ifdh cp --cp_maxretries=1 temp.mac ${filename}
  #cp temp.mac ${filename}
  done
}

n_evt_per_file=100
n_files=200
list=/pnfs/dune/scratch/users/mvicenzi/job_sub/macros/numu/macros_list.txt
exe=/pnfs/dune/scratch/users/mvicenzi/job_sub/FLArE 
out=/pnfs/dune/scratch/users/mvicenzi/FLArE_numu

ifdh rm ${exe}
ifdh cp ../../FLArE ${exe}

rm ${out}/*

list_fields=(0)
local_list="temp.txt"

if [ -f ${list} ]; then
  rm ${list}
fi
if [ -f ${local_list} ]; then
  rm ${local_list}
fi
touch $list
  
for i in "${!list_fields[@]}"; do
  
  echo "for B field of ${list_fields[i]} ..."
  field=${list_fields[i]}
  gen_single_macro $field $n_evt_per_file $n_files $local_list

done

ifdh rm $list
ifdh cp $local_list $list
source submit.sh $list $exe $out


