#!/bin/sh

. test_settings.sh

echo "-- target  ip address = $dest_ip"
echo "-- target mac address = $dest_mac"

echo "-- synchronize time"
ntpdate ntp.intra.rma.ac.be

#echo "-- bring eth0 down"
#ifconfig eth0 down

echo "-- start in 5s"
sleep 5

for packetlength in $test_packet_lengths
do
  for bitrate in $test_bitrates
  do
    echo "-- $packetlength bytes data at $bitrate Mbps"

    # refresh arp entry
    ping -c 1 $dest_ip > /dev/null

    # start packet generator
    sleep 1
    ./pgen_raw -i $cs_if   -c 1 -s $cs_number_of_packets   -l 0             -d $dest_mac -b $cs_bitrate -t 0
    ./pgen_raw -i $dest_if -c 1 -s $test_number_of_packets -l $packetlength -d $dest_mac -b $bitrate
    ./pgen_raw -i $cs_if   -c 1 -s $cs_number_of_packets   -l 0             -d $dest_mac -b $cs_bitrate -t 0

    # block until receiver has finished
    echo "-- synchronize with recorder"
    ./prec_raw_block -i $block_if -o /dev/null

    sleep 5
  done
done

#echo "-- bring eth0 up"
#ifconfig eth0 up  

