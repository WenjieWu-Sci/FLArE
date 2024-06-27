## Submitting a job on lxplus
**General Documentation**: https://batchdocs.web.cern.ch/index.html

**Quick Start Guide**: https://batchdocs.web.cern.ch/local/quick.html

**File Access Best Practices**: https://cern.service-now.com/service-portal?id=kb_article&n=KB0003076

### Single-job example
This example shows how to submit a single Geant4 (FLArE) job. Output logs must go to `/afs/cern.ch/`, while output files are transferred to `\eos` via xroot plugin.
Input files are transferred to the node using `transfer_input_file`. Note that in any case `/afs/cern.ch/` is visible from the nodes.
On the other hand, input files from `\eos` need to be copied by the executable itself (via eos xroot access).

Submission command: `condor_submit example.sub`

Status check: `condor_q`

#### File: `example.sub`
```
executable              = example.sh
arguments               = $(ClusterId) $(ProcId)
output                  = /afs/cern.ch/work/m/mvicenzi/public/LOGS/$(ClusterId)/out/welcome.$(ProcId).out
error                   = /afs/cern.ch/work/m/mvicenzi/public/LOGS/$(ClusterId)/err/welcome.$(ProcId).err
log                     = /afs/cern.ch/work/m/mvicenzi/public/LOGS/$(ClusterId)/log/welcome.$(ProcId).log
transfer_input_files    = /afs/cern.ch/work/m/mvicenzi/public/FLArE/lxplus_setup.sh,/afs/cern.ch/work/m/mvicenzi/public/FLArE/build/FLArE,/afs/cern.ch/work/m/mvicenzi/public/geometry.mac,/afs/cern.ch/work/m/mvicenzi/public/macro.mac,/afs/cern.ch/work/m/mvicenzi/public/FLArE/build/libFLArE_rdict.pcm
output_destination      = root://eosuser.cern.ch//eos/user/m/mvicenzi/HTCONDOR_OUTPUT/$(ClusterId)/
MY.XRDCP_CREATE_DIR     = True
queue 1
```
`$(ClusterId)` and `$(ProcId)` are expanded during execution. 
They're currently used for naming the output directories, as well as executable arguments. 
All small files needed by the executable (setup script, Geant4 macros, libraries) are transferred in the submission file.
Input ROOT files (such as GENIE `gst` files) are copied on the node by the executable itself.
Output is sent automatically to `\eos` via xroot protocol.

#### File: `example.sh`
```
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
```
The executable sets the environment and creates the log directories. Since those are based on the cluster id, they cannot be created in advance.
Note that all files (`lxplus_setup.sh`, `macro.mac`) are available locally because they were transferred by the submission file.
If a GENIE `gst` is needed, it must be transferred from within the executable.
