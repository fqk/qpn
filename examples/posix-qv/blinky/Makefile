##############################################################################
# Product: Makefile for QP/C, Blinky console, POSIX, GNU compiler
# Last updated for version 5.6.5
# Last updated on  2016-06-07
#
#                    Q u a n t u m     L e a P s
#                    ---------------------------
#                    innovating embedded systems
#
# Copyright (C) Quantum Leaps, LLC. All rights reserved.
#
# This program is open source software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Alternatively, this program may be distributed and modified under the
# terms of Quantum Leaps commercial licenses, which expressly supersede
# the GNU General Public License and are specifically designed for
# licensees interested in retaining the proprietary status of their code.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# Contact information:
# http://www.state-machine.com
# mailto:info@state-machine.com
##############################################################################
# examples of invoking this Makefile:
# building configurations: Debug (default), Release, and Spy
# make
# make CONF=rel
#
# cleaning configurations: Debug (default), Release, and Spy
# make clean
# make CONF=rel clean
# make CONF=spy clean

#-----------------------------------------------------------------------------
# location of the QP-nano framework (if not provided in an environemnt var.)
ifeq ($(QPN),)
QPN := ../../..
endif

#-----------------------------------------------------------------------------
# GNU toolset
#
CC    := gcc
CPP   := g++
LINK  := gcc   # for C programs
#LINK  := g++  # for C++ programs

MKDIR := mkdir -p
RM    := rm -f


#-----------------------------------------------------------------------------
# directories
#
# Project name is derived from the directory name
PROJECT := $(notdir $(CURDIR))

QP_PORT_DIR := $(QPN)/ports/posix-qv
APP_DIR     := .

VPATH = $(APP_DIR)

# include directories
INCLUDES  = -I. \
	-I$(QPN)/include \
	-I$(QP_PORT_DIR)


# defines
DEFINES =

#-----------------------------------------------------------------------------
# files
#

# C source files
C_SRCS := $(wildcard *.c)

# C++ source files
CPP_SRCS := $(wildcard *.cpp)
QP_SRCS := \
	qepn.c \
	qfn_posix.c

#-----------------------------------------------------------------------------
# build options for various configurations
#


# combine all the soruces...
VPATH += $(QPN)/source $(QP_PORT_DIR)
C_SRCS += $(QP_SRCS)

ifeq (rel, $(CONF)) # Release configuration ..................................

BIN_DIR := rel

CFLAGS = -c -Wall -ffunction-sections -fdata-sections \
	-O2 -fno-strict-aliasing $(INCLUDES) $(DEFINES) -pthread -DNDEBUG

CPPFLAGS = -c -Wall -W -O2 -ffunction-sections -fdata-sections \
	-O2 -fno-strict-aliasing $(INCLUDES) $(DEFINES) -pthread -DNDEBUG


else  # default Debug configuration ..........................................

BIN_DIR := dbg

CFLAGS = -c -Wall -W -g -ffunction-sections -fdata-sections \
	-O $(INCLUDES) $(DEFINES) -pthread

CPPFLAGS = -c -Wall -W -g -ffunction-sections -fdata-sections \
	-O $(INCLUDES) $(DEFINES) -pthread

endif


LINKFLAGS = -L$(QP_PORT_DIR)/$(BIN_DIR) -pthread \
	-Wl,-Map,$(BIN_DIR)/$(PROJECT).map,--cref,--gc-sections

#-----------------------------------------------------------------------------

C_OBJS       := $(patsubst %.c,   %.o, $(C_SRCS))
CPP_OBJS     := $(patsubst %.cpp, %.o, $(CPP_SRCS))

TARGET_BIN   := $(BIN_DIR)/$(PROJECT).bin
TARGET_EXE   := $(BIN_DIR)/$(PROJECT)
C_OBJS_EXT   := $(addprefix $(BIN_DIR)/, $(C_OBJS))
C_DEPS_EXT   := $(patsubst %.o, %.d, $(C_OBJS_EXT))
CPP_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(CPP_OBJS))
CPP_DEPS_EXT := $(patsubst %.o, %.d, $(CPP_OBJS_EXT))

# create $(BIN_DIR) if it does not exist
ifeq ("$(wildcard $(BIN_DIR))","")
$(shell $(MKDIR) $(BIN_DIR))
endif

#-----------------------------------------------------------------------------
# rules
#

all: $(TARGET_EXE)
#all: $(TARGET_BIN)

$(TARGET_BIN): $(TARGET_EXE)
	$(BIN) -O binary $< $@

$(TARGET_EXE) : $(C_OBJS_EXT) $(CPP_OBJS_EXT)
	$(LINK) $(LINKFLAGS) -o $@ $^

$(BIN_DIR)/%.d : %.c
	$(CC) -MM -MT $(@:.d=.o) $(CFLAGS) $< > $@

$(BIN_DIR)/%.d : %.cpp
	$(CPP) -MM -MT $(@:.d=.o) $(CPPFLAGS) $< > $@

$(BIN_DIR)/%.o : %.s
	$(AS) $(ASFLAGS) $< -o $@

$(BIN_DIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%.o : %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

# include dependency files only if our goal depends on their existence
ifneq ($(MAKECMDGOALS),clean)
  ifneq ($(MAKECMDGOALS),show)
-include $(C_DEPS_EXT) $(CPP_DEPS_EXT)
  endif
endif

.PHONY : clean
clean:
	-$(RM) $(BIN_DIR)/*
	
show:
	@echo PROJECT  = $(PROJECT)
	@echo CONF     = $(CONF)
	@echo VPATH    = $(VPATH)
	@echo C_SRCS   = $(C_SRCS)
	@echo CPP_SRCS = $(CPP_SRCS)
	@echo C_OBJS_EXT   = $(C_OBJS_EXT)
	@echo C_DEPS_EXT   = $(C_DEPS_EXT)
	@echo CPP_DEPS_EXT = $(CPP_DEPS_EXT)
