#!/bin/bash

export prodname=$1

export dir="/eos/user/${USER:0:1}/${USER}/HTCONDOR_OUTPUT/${prodname}/${prodname}_*.root"
export out="/eos/user/${USER:0:1}/${USER}/HTCONDOR_OUTPUT/${prodname}/${prodname}.root"
list=""

for f in $(ls ${dir});
do
	#echo $f
	list+=" ${f}"
done

#echo $list
echo "Merging into ${out}"

if test -f "$out"; then
    echo "$out exists. Removing old file"
    rm $out
fi

hadd ${out} ${list}
