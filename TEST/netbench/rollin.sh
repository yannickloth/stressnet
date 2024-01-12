#!/bin/sh

scp -r root@172.16.206.51:netbench/meas*.txt .
chown wmees:wmees meas*.txt


