export name=$1

list=""
histdir="/dune/data/users/${USER}/muon_studies"
out="${histdir}/${name}_fid_muons.root" 
dir="/dune/data/users/${USER}/muon_studies/${name}/${name}*.root"

for f in $(ls ${dir});
do
	#echo $f
	list+=" ${f}"
done

#echo $list
echo "Merging into ${out}"

mkdir -p $histdir

if test -f "$out"; then
    echo "$out exists. Removing old file"
    rm $out
fi

hadd ${out} ${list}
