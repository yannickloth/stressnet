#!/bin/sh

# -----
# target
# -----

# define IP address
export dest_ip=10.0.0.1

# extract MAC address for this IP address
ping -c 1 $dest_ip > /dev/null
export dest_mac=`arp -a | grep $dest_ip | cut -d ' ' -f 4`

# define the interface to use for reaching the target
export dest_if=eth1

# define the interface to use for blocking (waiting for target)
export block_if=eth1

# -----
# clock sync
# -----

# define the bitrate used for the clock sync packets
export cs_bitrate=100000

# define the number of clock sync packets 
export cs_number_of_packets=1000

# define the interface to use for clock sync
export cs_if=eth1

# -----
# set of tests
# -----

# define the packet length values that will be used
export test_packet_lengths="0 100 500 1000 1472"

# define the bitrate values that will be used
#export test_bitrates="10000000 20000000 50000000 90000000 100000000"
export test_bitrates="1000000 5000000 9000000 10000000 11000000"

# define the number of test packets 
#export test_number_of_packets=100000
export test_number_of_packets=10000

# define the size of receive buffer to pre-allocate
export test_preallocate=`echo "$test_number_of_packets + 2 * $cs_number_of_packets + 1" | bc`

