#!/bin/sh

echo "-- synchronize time"
ntpdate ntp.intra.rma.ac.be

#echo "-- bring eth0 down"
#ifconfig eth0 down

echo "-- remove old test results"
rm -f meas_*.txt

echo "-- start prec in 5s"
sleep 5

for packetlength in 0 100 500 1000 1472
do
  for bitrate in 10000000 20000000 50000000 90000000 100000000
  do
    echo "-- $packetlength bytes data at $bitrate Mbps"

    # start packet recorder
    ./prec_udp -o meas_$packetlength.$bitrate.txt -w 200 -p 14000 $*

    # release packet generator
    echo "-- release generator"
    ./pgen_raw -i eth1 -c 1 -s 5 -l 0 -d "ff:ff:ff:ff:ff:ff" -b 1000000
  done
done

#echo "-- bring eth0 up"
#ifconfig eth0 up
