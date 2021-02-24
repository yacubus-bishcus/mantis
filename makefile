PAR_TARGET := mantis_mpi

ACROLIBS := libmantisRoot.so

##################
#  G4 Makefile  #
##################
cp mantis.cc $PAR_TARGET.cc
name := $(PAR_TARGET)

G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

 .PHONY: all
all: lib/$(ACROLIBS) lib bin

include $(G4INSTALL)/config/binmake.gmk#.NO_VISUALIZATION

CXX = g++

G4V := $(shell geant4-config --version)
G4VS := $(subst ., ,$(G4V))
G4MAJV := $(word 1,$(G4VS))
G4MINV := $(word 2,$(G4VS))
G4SUBV := $(word 3,$(G4VS))
CXXFLAGS := -DG4MAJV=$(G4MAJV) -DG4MINV=$(G4MINV) -DG4SUBV=$(G4SUBV)

CXXFLAGS += $(subst -Wshadow,,$(CXXFLAGS))

CXXFLAGS += -O3 -std=c++11
CXXFLAGS += -fPIC


##########
#  ROOT  #
##########

ROOTINCLUDES = -I$(shell root-config --incdir)
ROOTDISTLIBS = $(shell root-config --nonew --libs --glibs)

CPPFLAGS += $(ROOTINCLUDES)
LDLIBS +=  $(ROOTDISTLIBS) -L./lib -lmantisRoot

MANTIS_ROOT_FILES = $(wildcard include/*.rhh)
MANTIS_ROOT_FILES += include/runMetadata.hh

lib/mantisDict.o : lib/mantisDict.cc
	@echo -e "\nBuilding $@ ...\n"
	@$(CXX) -fPIC $(CXXFLAGS) $(ROOTINCLUDES) -I. -c -o $@ $<

lib/libmantisRoot.so : lib/mantisDict.o
	@echo -e "\nBuilding the mantis ROOT library ...\n"
	@$(CXX) $(ROOTDISTLIBS) -shared -o $@  $^

lib/mantisDict.cc : $(MANTIS_ROOT_FILES) include/RootLinkDef.hh
	@echo -e "Generating the mantis ROOT dictionary ..."
	@rootcint -f $@ -c $^


#########################
#  Parallization / MPI  #
#########################

# Set the Open MPI C++ compiler name
CXX := mpic++
MANTIS_MPI_ENABLED := 1

# Necessary flags for parallel compilation
CPPFLAGS += -I$(MANTIS_MPIHOME)/include/ \
            -DMANTIS_MPI_ENABLED

.PHONY:

install:
	@echo -e "\nInstalling the mantis libary in $(MANTIS_TOPDIR)/lib/ ..."
	@cp -v $(G4WORKDIR)/tmp/$(G4SYSTEM)/mantis/libmantis.so $(MANTIS_TOPDIR)/lib/

libclean:
	@echo -e "\nCleaning up the mantis libraries ...\n"
	@rm -f lib/*
