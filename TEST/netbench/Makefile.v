#=======================================================================
#@V@:Note: File automatically generated by VIDE - 2.00/10Apr03! (g++).
# Generated 18:16:19 09 Feb 2004
# This file regenerated each time you run VIDE, so save under a
#    new name if you hand edit, or it will be overwritten.
#=======================================================================

# Standard defines:
CC  	=	g++
LD  	=	g++
WRES	=	windres
CCOMP	=	gcc
HOMEV	=	
VPATH	=	$(HOMEV)/include
oDir	=	.
Bin	=	.
libDirs	=
incDirs	=
LD_FLAGS =	-s -mwindows
LIBS	=	
C_FLAGS	=	-O

SRCS	=\
	c_eth.cc\
	c_pdu.cc\
	c_eth.hh\
	c_pdu.hh

EXOBJS	=\
	$(oDir)/c_eth.o\
	$(oDir)/c_pdu.o\
	$(oDir)/c_eth.o\
	$(oDir)/c_pdu.o

ALLOBJS	=	$(EXOBJS)
ALLBIN	=	$(Bin)/netbench.exe
ALLTGT	=	$(Bin)/netbench.exe

# User defines:

#@# Targets follow ---------------------------------

all:	$(ALLTGT)

objs:	$(ALLOBJS)

cleanobjs:
	rm -f $(ALLOBJS)

cleanbin:
	rm -f $(ALLBIN)

clean:	cleanobjs cleanbin

cleanall:	cleanobjs cleanbin

#@# User Targets follow ---------------------------------


#@# Dependency rules follow -----------------------------

$(Bin)/netbench.exe: $(EXOBJS)
	$(LD) -o $(Bin)/netbench.exe $(EXOBJS) $(incDirs) $(libDirs) $(LD_FLAGS) $(LIBS)