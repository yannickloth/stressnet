

#COMPILE_FLAGS = -Wall -ggdb 
#BUILD_FLAGS   = -ggdb 

COMPILE_FLAGS = -Wall -O
BUILD_FLAGS   = -O

EXECUTABLES   = pgen_raw pgen_udp prec_raw prec_raw_block prec_udp

FILES = c_pdu.o c_addr.o \
        c_macaddr.o c_eth.o \
        c_ipaddr.o c_ip.o \
        c_udp.o \
        c_data.o \
        c_ptx.o c_ptx_raw.o c_ptx_udp.o \
        c_prx.o c_prx_raw.o c_prx_udp.o 

default: $(FILES) $(EXECUTABLES)

pgen_raw: $(FILES) pgen_raw.o
	gcc $(BUILD_FLAGS) -o pgen_raw $(FILES) pgen_raw.o -lpcap -lpthread -lstdc++

pgen_udp: $(FILES) pgen_udp.o
	gcc $(BUILD_FLAGS) -o pgen_udp $(FILES) pgen_udp.o -lpcap -lpthread -lstdc++

prec_raw: $(FILES) prec_raw.o
	gcc $(BUILD_FLAGS) -o prec_raw $(FILES) prec_raw.o -lpcap -lpthread -lstdc++

prec_raw_block: $(FILES) prec_raw_block.o
	gcc $(BUILD_FLAGS) -o prec_raw_block $(FILES) prec_raw_block.o -lpcap -lpthread -lstdc++

prec_udp: $(FILES) prec_udp.o
	gcc $(BUILD_FLAGS) -o prec_udp $(FILES) prec_udp.o -lpcap -lpthread -lstdc++

.cc.o:
	gcc -c $(COMPILE_FLAGS) -I/usr/include/pcap $<

depend:
	gcc -MM -I/usr/include/pcap *.cc > .depend
	
clean:
	rm *.o $(EXECUTABLES)

strip:
	strip $(EXECUTABLES)
	
-include .depend	
