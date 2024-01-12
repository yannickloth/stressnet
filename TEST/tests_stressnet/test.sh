#!/bin/bash
export stressnetdir="/home/yannick/Coding/eclipse_workspace/stressnet/Rp3";
export dumpsdir="/home/yannick/dumps"
export minBitrate="10000"
export medianBitrate="50000"
export maxBitrate="250000"
#for((a=minBitrate;a<=medianBitrate;a=a+5000))
#do
#	$stressnetdir/stressnet -ieth0 -m$dumpsdir/udp1000.cap -b$a -q200000-r20000
#	echo "Bitrate done: $bitrate"
#done
for((a=medianBitrate;a<=maxBitrate;a=a+5000))
do
	$stressnetdir/stressnet -ieth0 -m$dumpsdir/udp1000.cap -b$a -q400000 -r20000
	echo "Bitrate done: $a"
done
