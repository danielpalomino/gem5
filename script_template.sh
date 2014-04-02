#/bin/bash

runbench()
{
BENCH=$1
CORES=$2
	(
		echo ${BENCH}${CORES};
		time nice -n10 ./build/ALPHA/gem5.opt -m -d ./parsec/${BENCH}${CORES} ./configs/example/fs.py -n $CORES --cpu-type=timing --caches --l2cache --script=./parsec-scripts/${BENCH}_${CORES}c_simsmall.rcS #>parsec/${BENCH}${CORES}\.log 2>parsec/${BENCH}${CORES}.err;
	)

}

for j in x264
#for j in bodytrack
#for j in blackscholes
#for j in ferret
#for j in swaptions
do
#	runbench $j 8 &
#	sleep 1
#	runbench $j 6 &
#	sleep 1
	runbench $j 4 &
#	sleep 1
#	runbench $j 2 &
#	sleep 1
#	runbench $j 4 &
	wait
done

