#!/bin/bash
sector=0
for j in `seq 1 6`
do
	drivename="/virtual"$j
	cd $drivename
	for i in `seq 0 9`
	do
		trackname="Track"$i
		mkdir $trackname
		cd $trackname
		for k in `seq 0 99`
		do
			sectorname="Sector"$sector
			echo This is Sector $sector of Surface $j and Track $i > $sectorname
			sector=`expr $sector + 1`
		done
		cd ..
	done
	cd ..
done
