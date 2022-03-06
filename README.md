# FLArE
Simulation code for R&D of the FLArE detector

## To compile the code
* Create a new directory to contain the executables (assume the path is /path/to/build)
* Assume the path to the source code is /path/to/source
* To compile, you need to go to the build directory `cd /path/to/build`
* And then `cmake /path/to/source`
* Finally `make`

Things get complicated with importing GENIE results to the Geant4 Application. What we're doing now is to link to GENIE libraries so we can have a dictionary for the ROOT file format, read in the TTree record and then loop over the particles in the event record and for those with status code == 1 (those particles coming out of the nucleus) and call Geant4's PrimaryVertex. Not only GENIE libraries, there are a bunch of another packages needed in order to link GENIE libraries. 
* Geant4 v4_10_6_p01c
* ROOT v6_22_06a
* GENIE v3_00_06k
* log4cpp v1_1_3c
* gsl v2_6a
* libxml2 v2_9_10a
* pythia6 v6_4_28r

## Macro command
* `/det/material`, option for detector material, choose from `LAr` (default) and `LKr`, run before `/run/initialize`
* `/det/saveGdml`, option for saving detector geometry in a GDML file, `false` as default, run before `/run/initialize`
* `/genie/useGenie`, option for PrimaryGeneratorMessenger, set `true` to read neutrino interactions from GENIE
* `/genie/genieInput`, if `useGenie is true`, give which `.ghep.root` file to read GENIE events
* `/genie/genieIStart`, if `useGenie is true`, give the start position of the file to read GENIE events
* `/histo/fileName`, option for AnalysisManagerMessenger, set name of the file saving all analysis variables
* `/histo/saveEvd`, if `true` save spatial distribution of energy deposition, `false` in default to save space

## Run simulation of leptons
There are six `.mac` macros in `macros` directory: LAr_e-.mac, LAr_mu-.mac, LAr_tau-.mac, LKr_e-.mac, LKr_mu-.mac, LKr_tau-.mac.
As denoted by the name, they're used to simulate e-/mu-/tau- leptons in a LAr or LKr detector. To run a simulation, just do `./FLArE macros/LAr_e-.mac`.
If you want to modify the lepton energy you intend to simulate, modify the parameters inside the mac scripts.

Some initial results can be found at [cern indico](https://indico.cern.ch/event/1095064/contributions/4621162/attachments/2349156/4006611/20211118%40FLArEDetectorSimulation.pdf)
