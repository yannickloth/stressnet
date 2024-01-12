
default: httpclient httpmeas bwmeas cpumeas

httpclient: httpclient.cc
	gcc -Wall -o httpclient httpclient.cc -lstdc++ -lpthread

httpmeas: httpmeas.cc
	gcc -Wall -ggdb -o httpmeas httpmeas.cc -lstdc++ -lpcap

bwmeas: bwmeas.cc
	gcc -Wall -o bwmeas bwmeas.cc -lstdc++ -lpthread -lpcap

cpumeas: cpumeas.cc
	gcc -Wall -o cpumeas cpumeas.cc -lstdc++ -lpthread

clean:
	rm -f httpclient httpmeas bwmeas cpumeas    

strip:
	strip httpclient
	strip httpmeas 
	strip bwmeas 
	strip cpumeas   
	
