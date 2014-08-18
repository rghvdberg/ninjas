#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = d_parameters

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ExamplePluginParameters.cpp.o

OBJS_UI  = \
	ExampleUIParameters.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(LINUX),true)
all: jack ladspa dssi lv2_sep vst
else
all: ladspa dssi lv2_sep vst
endif

# --------------------------------------------------------------
