# Description

This directory contains scripts for submitting batch jobs to simulate numu interactions in FLArE.
* `batch_script.sh` is the script that is actually being submitted and executed on the worker nodes. 
   Worker nodes have a limited scope, so the script takes care of:
   1. copying over from `pnfs` all inputs (including the executable);
   2. running the executable;
   3. copying back to `pnfs` output and log files.  

* `submit.sh` is the script that handles the job submission by calling `jobsub_submit`.
   It sets the required computing resources and passes the input filepaths to `batch_script.sh`.

* `numu_job.sh` is the main script: 
   1. it sets production parameters (number of files, number of events) and paths;
   2. it generates several `.mac` files starting from a template, keeping track of them in a list;
   3. it passes the lists of configuration files and other paths to `submit.sh`.

* `glob_output.sh` is the script that retrieves all the job output files in `/pnfs/dune/scratch`.
   The files are copied over to `dune\data` for storage.

* `merge_outputs.sh` is the script that merges all the output files into a single one.

## Instructions:
* The `.mac` template is 
```
/control/execute ${geomacro}

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
```   
The script relies on GENIE-simulated numu neutrino vertices stored in `numu_kling_ar40_e5000.ghep.root`.
In order for this file to be visibile on the worker nodes, `batch_script.sh` copies it over from an hardcoded `pnfs` location:
```
ifdh cp /pnfs/dune/persistent/users/mvicenzi/numu/numu_kling_ar40_e5000.ghep.root numu_kling_ar40_e5000.ghep.root
```
* The current implementation requires the user to provide the name of the geometry macro from `macro\geometry_options` by writing it in `numu_job.sh`. The name of this file is used to create the output directory as well as for the filenames themselves. It is the only argument that is needed by `glob_output.sh` and `merge_outputs.sh`.
* `${istart}` is set according to the number of files requested and the number of events per file chosen, in order to avoid using the same events.
* Most of the paths are currently hardcoded to point to `/pnfs/dune/scratch/users/mvicenzi` to store (temporary) submission files as well as the output files. They can be changed as necessary, but they need to be on `pnfs` to be accessible by the worker nodes with `ifdh cp`.
  
