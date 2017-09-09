#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ninjas

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	Ninjas.cpp.o \
	ADSR.cpp.o \
	Mixer.cpp.o \
	Sample.cpp.o \
	Slice.cpp.o \
	Stack.cpp.o \
	Voice.cpp.o
	

	

OBJS_UI  = \
	NinjasUI.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif
endif

ifeq ($(HAVE_DGL),true)
TARGETS += lv2_sep
else
TARGETS += lv2_dsp
endif

TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
