#!/bin/bash

function genenerate_macros {
  num_evt_per_file=$1; shift
  num_file=$1; shift
  loc_list=$1; shift  
  geomacro=$1; shift
  
  geoname=${geomacro%%.*}
  echo $geoname

  for i in `seq 1 1 ${num_file}`
  do
    seed1=$((42+i))
    seed2=$((47+i))

    istart=$((num_evt_per_file*i-num_evt_per_file))
    filename="/pnfs/dune/scratch/users/wenjiewu/FLArE/macro_savePM/nutau_macro/${geoname}_Fid_${i}.mac"
    outputfile="nutau_${geoname}_fid_${i}.root"
    if [ -f ${filename} ]; then
      echo "${filename} exists, delete file first"
      ifdh rm ${filename}
    fi
    echo $filename >> $loc_list
    rm temp.mac
cat << EOF >> temp.mac
/control/execute ${geomacro}

/random/setSeeds ${seed1} ${seed2}
/run/initialize

/genie/useGenie true
/genie/genieInput nutau_bai_ar40_e2000.ghep.root
/genie/genieIStart ${istart}

/histo/addDiffusion false
/histo/saveHit false
/histo/save3DEvd true
/histo/save2DEvd false
/histo/circleFit true
/histo/fileName ${outputfile}

/run/beamOn ${num_evt_per_file}
EOF
 
  ifdh cp --cp_maxretries=1 temp.mac ${filename}
  done
}

n_evt_per_file=200
n_files=500
geomacro="FLArE_only.mac"
geoname=${geomacro%%.*}
geofile="/pnfs/dune/scratch/users/wenjiewu/FLArE/macro_savePM/${geomacro}"
list="/pnfs/dune/scratch/users/wenjiewu/FLArE/macro_savePM/nutau_${geoname}_macros_list.txt"
exe="/pnfs/dune/scratch/users/wenjiewu/FLArE/FLArE"
out="/pnfs/dune/scratch/users/wenjiewu/FLArE/nutau/${geoname}"

ifdh rm ${exe} 
ifdh rm ${geofile}
ifdh cp ../../FLArE ${exe}
ifdh cp ../../macros/geometry_options/${geomacro} ${geofile}
mkdir -p ${out}
rm ${out}/*

local_list="temp.txt"

if [ -f ${list} ]; then
  rm ${list}
fi
if [ -f ${local_list} ]; then
  rm ${local_list}
fi
touch $list
  
genenerate_macros $n_evt_per_file $n_files $local_list $geomacro

ifdh rm $list
ifdh cp $local_list $list
source submit.sh $list $geofile $exe $out


