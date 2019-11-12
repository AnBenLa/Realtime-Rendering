# -*- Makefile -*-

# $Id: Makefile,v 1.1 2008-04-10 09:49:21 guni4317 Exp $

default: targets

UNAME := $(shell uname -s)

prefix =..
include $(prefix)/defines.mk

CXXINCLUDE	+=-I$(prefix)/include
ifeq ($(UNAME), Darwin)
LIBS		+=-framework OpenGL
else
LIBS		+=-lGL
endif

EXTRA_LIBS	+=-L$(prefix)/ -Wl,-rpath,.

HEADER =\
	../include/glwindow.hpp \
        $(NULL)


SOURCE_LIB =\
	glwindow.cpp \
	$(NULL)

SOURCE =$(SOURCE_LIB) $(SOURCE_CLIB)

OBJECTS_LIB =$(SOURCE_LIB:.cpp=.o)

GLUTWRITERLIB	=libfglwindow.so

targets:	$(GLUTWRITERLIB)

$(GLUTWRITERLIB): $(OBJECTS_LIB)
	-$(RM) $@
	$(CXX) $(LDXXFLAGS) -shared -o $(prefix)/$@ $(OBJECTS_LIB) -lm

$(PROGRAMS):	% : %.o
	-$(RM) $@
	$(CXX) $(LDXXFLAGS) -o $(prefix)/$@ $< $(LIBS) $(EXTRA_LIBS)

realclean:: clean
	-$(RM) $(GLUTWRITERLIB)
	-$(RM) $(prefix)/$(PROGRAMS)

include $(prefix)/rules.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),realclean)
-include $(OBJECTS:.o=.d)
endif
endif

# $Id: Makefile,v 1.1 2008-04-10 09:49:21 guni4317 Exp $
