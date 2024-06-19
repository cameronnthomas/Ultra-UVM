# This is the SystemC Makefile to compile a general SystemC code
#
SYSTEMC=/home/ctrca/systemc
SYSTEMC_ARCH=linux64

UVM_SYSTEMC=/home/ctrca/uvm-systemc

LIB_DIRS= -L. -L$(SYSTEMC)/lib-$(SYSTEMC_ARCH) -L$(UVM_SYSTEMC)/lib-$(SYSTEMC_ARCH)

#Include directories.
INCLUDE_DIRS = -I. -I$(SYSTEMC)/include -I$(UVM_SYSTEMC)/include

#header files used, for dependency checking
HEADERS = uvm_agent.h uvm_driver_monitor.h uvm_scoreboard.h uvm_sequence.h uvm_test.h

#source files used, for dependency checking
SOURCES = uvm_top.cpp

DEPENDENCIES = \
	Makefile \
	$(HEADERS) \
	$(SOURCES)

LIBS = -luvm-systemc -lsystemc -lm

TESTS = hello

all: $(TESTS)

$(TESTS): $(DEPENDENCIES)
	g++ -o $@ $(SOURCES) $(INCLUDE_DIRS) $(LIB_DIRS) $(LIBS)

clean:
	rm -f $(TESTS)  
