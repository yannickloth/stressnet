. test_settings.sh

export nofalerts=`less $testdir/snortstderr.txt | grep ALERTS | cut -d ' ' -f 2`
export nofreceived=`less $testdir/snortstderr.txt | grep Snort\ received | cut -d ' ' -f 3`
export nofanalyzed=`less $testdir/snortstderr.txt | grep Analyzed | cut -d ' ' -f 6 | cut -d '(' -f 1`
export nofdropped=`less $testdir/snortstderr.txt | grep Dropped | cut -d ' ' -f 6 | cut -d '(' -f 1`
echo "$bitrate $nofreceived $nofalerts $nofanalyzed $nofdropped" >> result.txt

echo "Don't forget to copy the results file or to rename it, or it will be overwritten during the next execution!"
