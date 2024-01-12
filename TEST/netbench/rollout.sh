#!/bin/sh

( cd .. ; \
  scp -r netbench root@172.16.206.51: ; \
  scp -r netbench root@172.16.206.52:   \
)
