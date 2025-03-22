#!/bin/bash

function gen_single_particle_macros {
  particle=$1; shift
  particle_momentum=$1; shift
  particle_kin=$1; shift
  field=$1; shift
  num_evt_per_file=$1; shift
  num_file=$1; shift
  local_list=$1; shift  
  
  for i in `seq 1 1 ${num_file}`
  do
    seed=$((42+i))
    filename="/pnfs/dune/scratch/users/mvicenzi/job_sub/macros/muon_studies/${particle}_momentum_${particle_momentum}GeV_B_${field}T_${i}.mac"
    outputfile="${particle}_momentum_${particle_momentum}GeV_B_${field}T_${i}.root"
    if [ -f ${filename} ]; then
      echo "${filename} exists, delete file first"
      rm ${filename}
    fi
    echo $filename >> $local_list
cat << EOF >> ${filename}
/det/material LAr
/det/saveGdml false
/det/field ${field} tesla

/run/initialize

/random/setSeeds ${seed}

/gen/select gun
/gps/pos/type Point
/gps/pos/centre 0 0 2.5 m
/gps/direction 0 0 1
/gps/particle ${particle}
/gps/ene/mono ${particle_kin} GeV

/histo/saveEvd false
/histo/saveHit false
/histo/circleFit true
/histo/fileName ${outputfile}

/run/beamOn ${num_evt_per_file}
EOF
  done
}

function gen_batch_macros {
  n_evt_per_file=$1; shift
  n_files=$1; shift
  particle="mu-"
  particle_mass=0.105658
  list_particle_momentum=(20 30)
  list_fields=(0 1)
  
  list="/pnfs/dune/scratch/users/mvicenzi/job_sub/macros/muon_studies/macros_list.txt"
  local_list="temp.txt"
  if [ -f ${list} ]; then
      rm ${list} ${local_list}
  fi
  touch $list
  
  for i in "${!list_particle_momentum[@]}"; do
    echo ""
    echo "generating macro for mu- files of ${list_particle_momentum[i]} ..."
    particle_momentum=${list_particle_momentum[i]}
    particle_kin=$(echo "scale=2;sqrt($particle_mass*$particle_mass+$particle_momentum*$particle_momentum)-$particle_mass" | bc)
    for j in "${!list_fields[@]}"; do
      echo "for B field of ${list_fields[j]} ..."
      field=${list_fields[j]}
      gen_single_particle_macros $particle $particle_momentum $particle_kin $field $n_evt_per_file $n_files $local_list
    done
  done

  cp $local_list $list
}

num_of_evt_per_file=$1
num_of_files=$2

gen_batch_macros $num_of_evt_per_file $num_of_files
