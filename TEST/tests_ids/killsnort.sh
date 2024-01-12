#!/bin/bash
. test_settings.sh

#sleep $sleepbeforekill
sleep $* #the star * means that the arguments of the calling script are passed to this application (sleep)
echo "    ___killing all snort processes if any___"
killall snort

