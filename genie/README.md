# Usage of GENIE
General steps to use GENIE to simulate neutrino interactions:
* Preparing event generator inputs: cross-section splines
* Event generation
  * Use `gevgen`, a generic GENIE event generation application, for simple event generation cases
  * Event generation for realistic experimental setups: flux driver interface `GFluxI`, geometry navigation driver interface `GeomAnalyzerI`

## FPF

## DUNE ND
* ND LAr geometry was found in this git repository:
```
https://github.com/DUNE/dunendlar/tree/develop/dunendlar/Geometry/gdml
```
The one used here was saved in `analysis/gdml/nd_hall_only_lar.gdml`.

* Neutrino flux can be found here
```
/cvmfs/dune.osgstorage.org/pnfs/fnal.gov/usr/dune/persistent/stash/Flux/g4lbne/v3r5p4/QGSP_BERT/OptimizedEngineeredNov2017/neutrino/flux/g4lbne_v3r5p4_QGSP_BERT_OptimizedEngineeredNov2017_neutrino_*root
```
There are 249 files in total, in `dk2nu` format.
