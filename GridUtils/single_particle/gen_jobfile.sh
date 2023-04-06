#!/bin/bash

function gen_single_particle_macros {
  particle=$1; shift
  particle_momentum=$1; shift
  particle_kin=$1; shift
  num_evt_per_file=$1; shift
  num_file=$1; shift
  for i in `seq 1 1 ${num_file}`
  do
    seed=$((42+i))
    filename="macros/LAr_${particle}_momentum_${particle_momentum}GeV_${i}.mac"
    outputfile="LAr_${particle}_momentum_${particle_momentum}GeV_${i}.root"
    if [ -f ${filename} ]; then
      echo "${filename} exists, delete file first"
      rm ${filename}
    fi
cat << EOF >> ${filename}
/det/material LAr
/det/saveGdml false

/run/initialize

/random/setSeeds ${seed}

/gps/pos/type Point
/gps/pos/centre 0 0 -2.5 m
/gps/direction 0 0 1
/gps/particle ${particle}
/gps/ene/mono ${particle_kin} GeV

/histo/saveEvd false
/histo/fileName ${outputfile}

/run/beamOn ${num_evt_per_file}
EOF
  done
}

function gen_batch_macros {
  n_evt_per_file=$1; shift
  n_files=$1; shift
  list_particles=("e-" "mu-" "tau-" "pi0" "pi-" "kaon-" "proton" "neutron" "gamma")
  list_particle_mass=(0.000511 0.105658 1.77686 0.134977 0.13957 0.493677 0.938272 0.939565 0)
  list_particle_momentum=(20 20 20 25 25 50 8 5 20)
  for i in "${!list_particles[@]}"; do
    echo ""
    echo "generating macro files of ${list_particles[i]} ..."
    particle_mass=${list_particle_mass[i]}
    particle_momentum=${list_particle_momentum[i]}
    particle_kin=$(echo "scale=2;sqrt($particle_mass*$particle_mass+$particle_momentum*$particle_momentum)-$particle_mass" | bc)
    gen_single_particle_macros ${list_particles[i]} $particle_momentum $particle_kin $n_evt_per_file $n_files
  done
}

if [ $# == 0 ]; then
  echo "Need an argument, use -h to get all available arguments"
elif [ $1 == "macro" ]; then
  echo "generating macro files for single particles ..."
  echo ""
  num_of_evt_per_file=200
  num_of_files=50
  gen_batch_macros $num_of_evt_per_file $num_of_files
elif [ $1 == "-h" ]; then
  echo "macro : generate macro files for single particles"
else
  echo "Need an argument, use -h to get all available arguments"
fi
