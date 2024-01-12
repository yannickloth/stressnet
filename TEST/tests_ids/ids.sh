#!/bin/bash
. test_settings.sh

echo "___removing old results___"
rm -f $testdir/snortstd*.txt 
rm -f $testdir/result.txt

echo "___killing existing snort processes if any___"
killall snort
killall sleep
killall bash\ killsnort.sh
	for ratio in $ratio
	do
		for ((repeat=1;repeat<=20;repeat++))
		do	
			for bitrate in $bitrates
			do
				echo "___Bitrate: $bitrate kbit/s___"
				export sleepbeforekill=`echo "8*$nbkbytes/$bitrate+20" | bc` #20 SHOULD be enough.  If syncronization fails, try a higher value
				bash killsnort.sh $sleepbeforekill &
				sleep 3	
				echo "___Launch snort___"
				$snortdir/snort -c $snortdir/../etc/snort.conf -l $snortdir/../log -i eth0 >$testdir/snortstdout.txt 2>$testdir/snortstderr.txt
		
				echo "___Parsing snort output files___"
				. parse.sh
		
				echo "___Generate releasing packet___"
				../netbench/pgen_raw -i eth0 -c 1 -s 5 -l 0 -d "ff:ff:ff:ff:ff:ff" -b 100000 
			done
		done
	done
killall sleep 
killall bash\ killsnort.sh
echo "___Finished the test.___"


