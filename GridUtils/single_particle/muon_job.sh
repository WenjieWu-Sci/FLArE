num_of_evt_per_file=50
num_of_files=100

source gen_muons.sh $num_of_evt_per_file $num_of_files

list=/pnfs/dune/scratch/users/mvicenzi/job_sub/macros/muon_studies/macros_list.txt
exe=/pnfs/dune/scratch/users/mvicenzi/job_sub/FLArE 
out=/pnfs/dune/scratch/users/mvicenzi/muon_studies/

rm ${exe}
cp ../../FLArE ${exe}

rm ${out}/*

source submit.sh $list $exe $out
