# XTL - the eXternalization Template Library (sample code)
# by Jose' Orlando Pereira, Universidade do Minho
# 
# jop@di.uminho.pt - http://gsd.di.uminho.pt/~jop
# 
# $Id: Makefile,v 1.2 2000/12/22 10:48:18 jop Exp $

TOPDIR  := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)/../..

# Edit this file to set the compiler/ compiler options and send
# the modified file to jop@di.uminho.pt
include ../Makefile.common

all: smart 

smart: smart.cpp
	$(CXX) $(CXXFLAGS) -o $@ $(INCLUDE) $<

dep:
	touch .depend
	makedepend -f .depend $(INCLUDE) *.cpp

clean:
	rm -f smart .depend .depend.bak

ifeq (.depend,$(wildcard .depend))
include .depend
endif
