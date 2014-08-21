#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = d_meters

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ExamplePluginMeters.cpp.o

OBJS_UI  = \
	ExampleUIMeters.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(LINUX),true)
all: jack dssi lv2_sep vst
else
all: dssi lv2_sep vst
endif

# --------------------------------------------------------------
