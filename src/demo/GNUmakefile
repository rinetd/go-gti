
# Setup gti build environment.
ifndef GTISDKROOT
#	include ../../Build/GTISDK.env
endif

CPU_ARCH ?=$(shell uname -m)
OS_TYPE  ?=$(shell uname)
CROSS_COMPILE ?= 0


ifeq ($(CROSS_COMPILE), 1)
$(info CROSS_COMPILE is $(CROSS_COMPILE))
FTDLIB = -l:libftd3xx.so -Wl,-rpath=$(LIBFTDI_PATH)
EXTRA_SYS_LIB =
else
CROSS_COMPILE=
# anticipate the library on an embedded platform is older
ifneq ($(CPU_ARCH),x86_64)
FTDLIB = -l:libftd3xx.so -Wl,-rpath=$(LIBFTDI_PATH)
EXTRA_SYS_LIB = -l:libstdc++.so.6
else
FTDLIB = -lftd3xx-static
EXTRA_SYS_LIB =
endif
endif

GTI_CC ?= g++

all: $(wildcard *.cpp) $(wildcard *.hpp)
	$(GTI_CC) -g -o liteDemo -std=c++11 *.cpp -I ../Include/ \
	-L ../Lib/$(OS_TYPE)/$(CPU_ARCH)/ -lGTILibrary-static $(FTDLIB) \
	$(EXTRA_SYS_LIB) -pthread

clean:
	rm liteDemo

