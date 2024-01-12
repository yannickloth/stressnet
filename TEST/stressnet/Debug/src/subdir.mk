################################################################################
# Automatically-generated file. Do not edit!
################################################################################

S_UPPER_SRCS += \
${addprefix $(ROOT)/src/, \
}

CPP_SRCS += \
${addprefix $(ROOT)/src/, \
dumpread.cpp \
licenseinfo.cpp \
stressnet.cpp \
}

CC_SRCS += \
${addprefix $(ROOT)/src/, \
}

C_SRCS += \
${addprefix $(ROOT)/src/, \
cmdline.c \
memmanagement.c \
processproperties.c \
timing.c \
}

C_UPPER_SRCS += \
${addprefix $(ROOT)/src/, \
}

CXX_SRCS += \
${addprefix $(ROOT)/src/, \
}

S_SRCS += \
${addprefix $(ROOT)/src/, \
}

# Each subdirectory must supply rules for building sources it contributes
src/%.o: $(ROOT)/src/%.c
	@echo 'Building file: $<'
	@echo g++ -O0 -g3 -Wall -c -fmessage-length=0 -o$@ $<
	@g++ -O0 -g3 -Wall -c -fmessage-length=0 -o$@ $< && \
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	g++ -MM -MG -P -w -O0 -g3 -Wall -c -fmessage-length=0  $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: $(ROOT)/src/%.cpp
	@echo 'Building file: $<'
	@echo g++ -O0 -g3 -Wall -c -fmessage-length=0 -o$@ $<
	@g++ -O0 -g3 -Wall -c -fmessage-length=0 -o$@ $< && \
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	g++ -MM -MG -P -w -O0 -g3 -Wall -c -fmessage-length=0  $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


