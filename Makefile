
INSTALL_LOCATION = ~/ROOT/slib

#__GXX_EXPERIMENTAL_CXX0X__ = 1
#__cplusplus = 201103L 

LINKDEFS = $(wildcard *LinkDef.h)

# Find class names basede on the LinkDef files present
CLASS_NAMES = $(subst LinkDef.h,, $(LINKDEFS) )
# Create list of source, object, pcm and shared lib files
DICT_SRCS   = $(subst LinkDef.h,Dict.cpp, $(LINKDEFS) )
DICT_OBJS   = $(subst LinkDef.h,Dict.o, $(LINKDEFS) )
DICT_PCMS   = $(subst LinkDef.h,Dict_rdict.pcm, $(LINKDEFS) )
DICT_SLIB   = $(subst LinkDef.h,.so, $(LINKDEFS) )

# define which binaries need to be build
BINARIES  = $(DICT_SLIB) WrapperVirt.so

ROOT_INCLUDE += $(shell root-config --incdir)
ROOT_LIBS += $(shell root-config --libs)

INCLUDE_DIR += -I/home/hovanes/GlueX/MCB/src -I$(ROOT_INCLUDE)

CXXFLAGS = 	-c -g -fPIC -Wall -O2 -std=c++11 -Wall -fmessage-length=0  $(INCLUDE_DIR)
LDFLAGS =	-g -shared -Wall -m64 

.SECONDARY: $(DICT_SRCS) $(DICT_OBJS) $(DICT_PCMS) $(DICT_SLIB)
		
%.so:  %Dict.o
	$(CXX) $(LDFLAGS) -o $@ $^  $(ROOT_LIBS)

%Dict.cpp: %.cpp %.hh %LinkDef.h 
	@echo "creating dictionary $* (.hh .cc) ..."
	rootcling -v -f $@ -c $^

	
# define what needs to be build 
all:	$(BINARIES)  

install: all 	
	@echo "installing to " $(INSTALL_LOCATION) 
	install -d $(INSTALL_LOCATION) 
	install -C $(DICT_PCMS) $(DICT_SLIB) $(INSTALL_LOCATION) 

clean:
	rm -f $(DICT_SRCS) $(DICT_OBJS) $(DICT_PCMS) $(DICT_SLIB) *.o *.so 

#WrapperVirt.o: WrapperVirt.cpp WrapperVirt.hh
#WrapperVirt.so: WrapperVirt.o
#	$(CXX) $(LDFLAGS) -o $@ $^  $(ROOT_LIBS)
#

	