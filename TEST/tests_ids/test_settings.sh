#!/bin/bash
#This file contains the configuration settings of the tests.  This is the only file to be modified through the tests.
#echo "Loading settings into memory."

export dumpdir="/home/yannick/dumps"

export stressnetdir="/home/yannick/Coding/eclipse_workspace/stressnet/Rp3"
#export bitrates="10000 20000 30000 40000 50000 60000 70000 80000 90000 100000 110000 120000 130000 140000 150000 160000 170000 180000 190000 200000"
export bitrates="100000"
export priority="0"
export fileM="$dumpdir/smtp_outside.dump"
#export fileM="$dumpdir/outside.tcpdump"
export fileN="$dumpdir/UDP_port_zero.dump"
export interface="eth0"
export ratio="999"
export quantity="200000"
#export repeat="1 2 3 4 5 6 7 8 9 10"
export nbkbytes="65000"

export testdir="/home/yannick/tests"
export snortdir="/home/yannick/Documents/snort-2.3.0/src"
export sleepbeforekill="60"

export netbenchdir=/home/yannick/Coding/netbench


