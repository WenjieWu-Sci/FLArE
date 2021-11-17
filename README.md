# FLArE
Simulation code for R&D of the FLArE detector

## To compile the code
* Create a new directory to contain the executables (assume the path is /path/to/build)
* Assume the path to the source code is /path/to/source
* To compile, you need to go to the build directory `cd /path/to/build`
* And then `cmake /path/to/source`
* Finally `make`

## Run simulation of leptons
There are six `.mac` macros in `macros` directory: LAr_e-.mac, LAr_mu-.mac, LAr_tau-.mac, LKr_e-.mac, LKr_mu-.mac, LKr_tau-.mac.
As denoted by the name, they're used to simulate e-/mu-/tau- leptons in a LAr or LKr detector. To run a simulation, just do `./FLArE macros/LAr_e-.mac`.
If you want to modify the lepton energy you intend to simulate, modify the parameters inside the mac scripts.

