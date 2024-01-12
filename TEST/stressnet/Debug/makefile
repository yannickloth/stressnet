################################################################################
# Automatically-generated file. Do not edit!
################################################################################

ROOT := ..

-include $(ROOT)/makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include $(SUBDIRS:%=%/subdir.mk)
-include objects.mk
-include $(DEPS)
-include $(ROOT)/makefile.defs

all: stressnet

stressnet: $(OBJS)
	@echo 'Building target: $@'
	g++ -L/usr/lib -o $@ $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building: $@'

clean:
	-$(RM) $(OBJS) $(DEPS) stressnet

.PHONY: all clean dependents

-include $(ROOT)/makefile.targets
