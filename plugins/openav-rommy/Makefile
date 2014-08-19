#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = roomy

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ArtyFxPluginRoomy.cpp.o

OBJS_UI  = \
	ArtyFxUiRoomy.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.ntk.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(LINUX),true)
all: jack ladspa dssi lv2_sep vst
else
all: ladspa dssi lv2_sep vst
endif

# --------------------------------------------------------------
