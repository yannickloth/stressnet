#!/bin/sh

export dest_ip=10.0.0.1
echo "-- target  ip address = $dest_ip"

ping -c 1 $dest_ip > /dev/null
export dest_mac=`arp -a | grep $dest_ip | cut -d ' ' -f 4`
echo "-- target mac address = $dest_mac"

echo "-- synchronize time"
ntpdate ntp.intra.rma.ac.be

#echo "-- bring eth0 down"
#ifconfig eth0 down

echo "-- start in 5s"
sleep 5

for packetlength in 16 100 500 1000 1472
do
  for bitrate in 10000000 20000000 50000000 90000000 100000000
  do
    echo "-- $packetlength bytes data at $bitrate Mbps"

    # refresh arp entry
    ping -c 1 $dest_ip > /dev/null

    # start packet generator
    sleep 1
    ./pgen_udp -c 1 -s 1000   -l 16            -D $dest_ip -b 100000 -t 0 -p 14000
    ./pgen_udp -c 1 -s 100000 -l $packetlength -D $dest_ip -b $bitrate    -p 14000
    ./pgen_udp -c 1 -s 1000   -l 16            -D $dest_ip -b 100000 -t 0 -p 14000

    # block until receiver has finished
    echo "-- synchronize with recorder"
    ./prec_raw_block -i eth1 -o /dev/null
    sleep 5
  done
done

#echo "-- bring eth0 up"
#ifconfig eth0 up  

