#/bin/bash

runbench()
{
BENCH=$1
CORES=$2
	(
		echo ${BENCH}${CORES};
		time nice -n10 ./build/ALPHA/gem5.opt -m -d ./daniel/${BENCH}${CORES} ./configs/example/fs.py -n $CORES --cpu-type=detailed --caches --l2cache --script=./daniel-scripts/${BENCH}_${CORES}c_simmedium.rcS --disk-image=image_ces265.img
	)

}

for j in ces265
do
	runbench $j 4 &
	wait
done

