#!/bin/sh
#To test new features, set the priority to 0 to avoid PC-hang-up !!!
#./Raxp/stressnet -ieth0 -m/home/yannick/dumps/smtp_outside.dump -r10 -p0 -q20000 -b10000
#./Raxp/stressnet -ieth0 -m/home/yannick/dumps/smtp_outside.dump -r10 -p0 -q200000 -b30000
#./Raxp/stressnet -ieth0 -m/home/yannick/dumps/smtp_outside.dump -r10 -p0 -q200000 -b50000
#./Raxp/stressnet -ieth0 -m/home/yannick/dumps/smtp_outside.dump -r10 -p0 -q200000 -b60000
#./Raxp/stressnet -ieth0 -m/home/yannick/dumps/smtp_outside.dump -r10 -p0 -q200000 -b70000
#./Raxp/stressnet -ieth0 -m/home/yannick/dumps/smtp_outside.dump -r10 -p0 -q200000 -b80000
./Rp3/stressnet -ieth0 -m/home/yannick/dumps/smtp_outside.dump -r10 -p0 -q200000 -b90000 -s
#./Rp3/stressnet -ieth0 -m/home/yannick/dumps/smtp_outside.dump -r10 -p0 -q1000000 -b0
#./Raxp/stressnet -ieth1 -m/home/yannick/dumps/smtp_outside.dump -r10 -p0 -q500000 -b150000 -s100000
#./Raxp/stressnet -ieth1 -m/home/yannick/dumps/smtp_outside.dump -r10 -p0 -q800000 -b200000 -s100000
#./Raxp/stressnet -ieth1 -m/home/yannick/dumps/smtp_outside.dump -r10 -p0 -q800000 -b250000 -s100000
#./Release/stressnet -ieth1 -m/home/yannick/dumps/udp1000r.cap -r10 -p0 -q800000 -b500000 -s1000000
#./stressnet -ieth1 -m/home/yannick/dumps/single164.dump -n/home/yannick/dumps/outside.tcpdump -r10 -p0 -q200000 -b50000 -s100000

#----------------------------------------

#Tested and OK:
#./stressnet -ieth1 -m/home/yannick/dumps/single164.dump -n/home/yannick/dumps/mpls-twolevel.cap -r1 -b10000 -p0 -q50000 -s100000
#./stressnet -ieth1 -m/home/yannick/dumps/single164.dump -n/home/yannick/dumps/mpls-twolevel.cap -r1 -b10000 -lsss.log -p0 -q20 -s100000
#./stressnet -ieth1 -m/home/yannick/dumps/single164.dump -n/home/yannick/dumps/single164.dump -r1 -b10000 -lsss.log -p80 -q20000 -s100000
#./stressnet -ieth1 -m/home/yannick/dumps/single164.dump -n/home/yannick/dumps/single164.dump -r5 -b10000 -lsss.log -p80 -q20000 -s100000

#----------------------------------------

#Tested, not OK, but not dangerous for computer:
#./stressnet -ieth1 -m/home/yannick/dumps/single164.dump -n/home/yannick/dumps/single164.dump -r0 -b10000 -lsss.log -p80 -q20000 -s100000

#----------------------------------------

#Tested and DANGEROUS:


#----------------------------------------

