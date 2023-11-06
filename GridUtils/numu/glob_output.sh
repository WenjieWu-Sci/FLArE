export name=${1}
export path="/pnfs/dune/scratch/users/${USER}/muon_studies/${name}"
export odir="/dune/data/users/${USER}/muon_studies/${name}/"

#create directory (if not existing)
mkdir -p $odir

#loop through list of output files
ID=0
for file in $( find ${path} -print | egrep '\.root$' );
do
        #echo $file
   	if test -f "${odir}/${name}_${ID}.root"; then
		ifdh rm ${odir}/${name}_${ID}.root
	fi
	
	ifdh cp ${file} ${odir}/${name}_fid_${ID}.root
  	((ID++))
done

echo "Project has ${ID} output files"

echo "ALL DONE!"
