#!/bin/bash

export GXMLPATH=$PWD:$GXMLPATH
export GDK2NUFLUXXML=LBNFFlux.xml

gevgen_fnal  \
-f /cvmfs/dune.osgstorage.org/pnfs/fnal.gov/usr/dune/persistent/stash/Flux/g4lbne/v3r5p4/QGSP_BERT/OptimizedEngineeredNov2017/neutrino/flux/g4lbne_v3r5p4_QGSP_BERT_OptimizedEngineeredNov2017_neutrino_00223.dk2nu.root,DUNEND,12,14 \
-g /dune/app/users/wenjiewu/FLArE_Dev/FLArE/analysis/gdml/nd_hall_only_lar.root \
-t volWorld \
-n 1000 \
-o neutrino \
-F mbox:-500,-300,200,500,300,1000 \
--cross-sections /cvmfs/larsoft.opensciencegrid.org/products/genie_xsec/v3_04_00/NULL/G1802a00000-k250-e1000/data/gxspl-FNALsmall.xml \
--event-generator-list Default

#setup genie_xsec v3_04_00 -q G1802a00000:e1000:k250
#-f /cvmfs/dune.osgstorage.org/pnfs/fnal.gov/usr/dune/persistent/stash/Flux/g4lbne/v3r5p4/QGSP_BERT/OptimizedEngineeredNov2017/neutrino/flux/g4lbne_v3r5p4_QGSP_BERT_OptimizedEngineeredNov2017_neutrino_00223.dk2nu.root, LBNE-NearDet_01, 10, -10, 12, -12 \
