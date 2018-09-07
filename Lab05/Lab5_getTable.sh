#!/bin/bash
currNum=$1
endNum=$2
while [ $currNum -le $endNum ]
do
	for count in {1..10}
	do
		./Lab5 $currNum
	done
	let "currNum = 10*currNum"
done
