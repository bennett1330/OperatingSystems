#!/bin/bash
argFile=$1
argNum=$2
totalNum=0
numIter=10
cmd="gcc -lpthread -o Lab7 $1"
$($cmd)
for count in $(seq 1 $numIter)
do
	cmd="./Lab7 $argNum"
	returnStr=$($cmd)
	echo $returnStr
	returnStr=${returnStr:10}
	timeNum=${returnStr:0:(-5)}
	totalNum=$(($totalNum+$timeNum))
done
avg=$(($totalNum/$numIter))
echo "average is $avg ms"
