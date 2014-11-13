#/bin/bash

runbench()
{
BENCH=$1
CORES=$2
	(
		echo ${BENCH}${CORES};
		time ./build/ALPHA/gem5.opt -m -d ./daniel/${BENCH}${CORES}_basketdrive1 ./configs/example/fs.py -n $CORES --cpu-type=detailed --caches --l2cache --script=./daniel-scripts/2x2/${BENCH}_${CORES}c_simmedium_basketdrive1.rcS --disk-image=image_ces265.img 
	)

}

for j in hm
do
	runbench $j 2 &
	wait
done

