#!/bin/bash

###-----------------------------------------
### DEFINITION OF PRODUCTION PARAMETERS
###----------------------------------------

# Production name for output directories
# This is used to place output logs and files
export prodname="test_nue"

# Define how many jobs, how many files
# Jobs will be placed in the same cluster
export n_jobs=2
export n_evt_per_job=1

# Path to FLArE build directory in /afs/cern.ch
export builddir="/afs/cern.ch/work/${USER:0:1}/${USER}/public/FLArE/build"
export flare="${builddir}/FLArE"
export setup="${builddir}/lxplus_setup.sh"
export libdict="${builddir}/libFLArE_rdict.pcm"

# Path to geometry macro in /afs/cern.ch
export geometry="${builddir}/macros/geometry_options/FPF_hall_Reference.mac"

# Path to genie gst file in /eos/
export geniegst="/eos/user/m/mvicenzi/genie/nue_kling_ar40_e5000.gst.root"

# Path to log directory
export logdir="/afs/cern.ch/work/${USER:0:1}/${USER}/public/LOGS"
# Path to /eos/user output directory
export outdir="/eos/user/${USER:0:1}/${USER}/HTCONDOR_OUTPUT"

###------------------------------------------------------------------
###------------------------------------------------------------------

function generate_macros {
  
  list=$1

  # for each job
  for i in `seq 1 1 ${n_jobs}`
  do

    # setup different seeds per jobs
    seed1=$((42+i))
    seed2=$((47+i))

    # select the start entry for the gst file
    istart=$((n_evt_per_job*i-n_evt_per_job))

    # select name for output file 
    outputfile="${prodname}_${i}.root"
    
    # final macro path  
    macfile="${logdir}/${prodname}/mac/${prodname}_${i}.mac"
    if [ -f ${macfile} ]; then
      echo "${macfile} exists, delete file first"
      rm ${macfile}
    fi

    # add macro path to list
    echo $macfile >> $list

    # write the macro itself!
    geo=`basename "${geometry}"`
    gst=`basename "${geniegst}"`

    rm temp.mac
    cat << EOF >> temp.mac
/control/execute ${geo}
    
/random/setSeeds ${seed1} ${seed2}
/run/initialize

/genie/useGenie true
/genie/genieInput ${gst}
/genie/genieIStart ${istart}

/histo/save3DEvd false
/histo/save2DEvd false
/histo/saveHit false
/histo/addDiffusion false
/histo/circleFit true
/histo/fileName ${outputfile}

/run/beamOn ${n_evt_per_job}
EOF
    
    # move the macro to final destination
    cp temp.mac ${macfile}
  done

}

###------------------------------------------------------------------
###------------------------------------------------------------------

function generate_submission_file {

  sub=$1; shift
  listpath=$1; shift

  cat << EOF >> ${sub}
executable              = batch_script.sh
arguments               = \$(ClusterId) \$(ProcId) ${geniegst}
output                  = ${logdir}/${prodname}/out/\$(ClusterId).\$(ProcId).out
error                   = ${logdir}/${prodname}/err/\$(ClusterId).\$(ProcId).err
log                     = ${logdir}/${prodname}/log/\$(ClusterId).\$(ProcId).log
transfer_input_files    = ${setup},${flare},${geometry},${libdict},${listpath}
output_destination      = root://eosuser.cern.ch/${outdir}/${prodname}/
MY.XRDCP_CREATE_DIR     = True
queue ${n_jobs}
EOF

}

###------------------------------------------------------------------
###------------------------------------------------------------------

# create log directories
echo "Creating log directories for: '$prodname'"
mkdir -p ${logdir}/${prodname}/out
mkdir -p ${logdir}/${prodname}/err
mkdir -p ${logdir}/${prodname}/log
mkdir -p ${logdir}/${prodname}/mac

# check if production with the same name already exists
if [ -d "${outdir}/${prodname}" ] && [ "$(ls -A ${outdir}/${prodname})" ]; then
  echo "WARNING: ${outdir}/${prodname} is not empty!"
  echo "Outputs for production ${prodname} already exist, delete them first!"
  exit
else
  if [ -d "${logdir}/${prodname}/out/" ] && [ "$(ls -A ${logdir}/${prodname}/out/)" ]; 
    then rm ${logdir}/${prodname}/out/*; fi
  if [ -d "${logdir}/${prodname}/err/" ] && [ "$(ls -A ${logdir}/${prodname}/err/)" ]; 
    then rm ${logdir}/${prodname}/err/*; fi
  if [ -d "${logdir}/${prodname}/log/" ] && [ "$(ls -A ${logdir}/${prodname}/log/)" ]; 
    then rm ${logdir}/${prodname}/log/*; fi
  if [ -d "${logdir}/${prodname}/mac/" ] && [ "$(ls -A ${logdir}/${prodname}/mac/)" ]; 
    then rm ${logdir}/${prodname}/mac/*; fi
fi


# generate all macros for each job
# as well as a list of their paths
echo "Generating list of .mac files..."
  
macrolist="macrolist.txt"
if [ -f ${macrolist} ]; then
  rm ${macrolist}
fi
touch $macrolist

generate_macros $macrolist

macropath="${logdir}/${prodname}/${macrolist}"
cp $macrolist ${macropath}

echo "Generating .sub file..."

subfile=${prodname}.sub
if [ -f ${subfile} ]; then
  rm ${subfile}
fi
touch ${subfile}

generate_submission_file $subfile $macropath

echo "Bookeeping submission files..."

cp $subfile ${logdir}/${prodname}

condor_submit $subfile

rm ${macrolist} ${subfile} temp.mac
