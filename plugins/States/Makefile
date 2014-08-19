#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = d_states

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ExamplePluginStates.cpp.o

OBJS_UI  = \
	ExampleUIStates.cpp.o

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
