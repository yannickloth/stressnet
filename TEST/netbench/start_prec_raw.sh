#!/bin/sh

. test_settings.sh

echo "-- synchronize time"
ntpdate ntp.intra.rma.ac.be

#echo "-- bring eth0 down"
#ifconfig eth0 down

echo "-- remove old test results"
rm -f meas_*.txt

echo "-- start prec in 5s"
sleep 5

for packetlength in $test_packet_lengths
do
  for bitrate in $test_bitrates
  do
    echo "-- $packetlength bytes data at $bitrate Mbps"

    # start packet recorder
    ./prec_raw -i $dest_if -a $test_preallocate -o meas_$packetlength.$bitrate.txt -w 200 $*

    # release packet generator
    echo "-- release generator"
    ./pgen_raw -i $block_if -c 1 -s 5 -l 0 -d "ff:ff:ff:ff:ff:ff" -b 1000000
  done
done

#echo "-- bring eth0 up"
#ifconfig eth0 up
