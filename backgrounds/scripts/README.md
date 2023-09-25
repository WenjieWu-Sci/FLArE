## Scripts
This directory contains ROOT macros to read the output of FLUKA simulations and produce histograms
that can be used as input for background studies.

The first two scrips are meant to be run at CERN (`lxplus`):
* `extract_fluence_data.C`: reads the `.txt` FLUKA output files and converts them into ROOT TTrees.
   Fluence data is directly available only for muons, but convers the entire FPF cavern.
* `extract_flux_data.C`: reads the `.dat` FLUKA output files and converts them into ROOT TTrees.
   Flux data contains complete description of all muons and neutrons crossing the entrance of the FPF cavern.
   It can be used to extract the flux for these particle species, but also other distributions.
 
The other scripts are used to produce the inputs histograms for the background studies.
These are saved in a ROOT filed and accessed by the generator class in Geant4.
* `make_muon_fluence_histos.C`: produces 2D projections of the muon fluence rates, with a focus on the FPF entrance.
* `make_muon_flux_histos.C`: produces the muon flux at the FPF entrance, binned in slices along the x-axis.   
* `make_neutron_flence_histos.C`: produces the 2D xy projection of the neutron fluence rate at the FPF entrance.   
* `make_neutron_flux_histos.C`: produces the neutron flux at the FPF entrance, binned in slices along the x-axis.   

### Samples
The original outputs of the FLUKA simulations produced by the CERN team are available in CERNBox.
Converted versions of those files are available on DUNE GPVMs.

|Description | CERNBox | DUNE GPVM |
|:--|:--|:--|
|Muon fluence, full FPF cavern      | `/eos/experiment/fpf-sim/FLUKA/2022`      | `/dune/data/users/mvicenzi/FPF_backgrounds/muons/fluence/` |
|Muon flux (v1), only FPF entrance  | `/eos/experiment/fpf-sim/FLUKA/2023/v1`   | `/dune/data/users/mvicenzi/FPF_backgrounds/muons/flux_v1/` |
|Muon flux (v2), only FPF entrance  | https://cernbox.cern.ch/s/7zxlKX1KRePn66s | `/dune/data/users/mvicenzi/FPF_backgrounds/muons/flux_v2/` |
|Neutron flux, only FPF entrance    | https://cernbox.cern.ch/s/7zxlKX1KRePn66s | `/dune/data/users/mvicenzi/FPF_backgrounds/neutrons/` |

_Note_: The statistics for muons changed from v1 to v2, so the two files require a different normalization factor. In particular, v2 has more statistics and it's the one produced alongside the neutron sample so they share the same normalization.
