# Copyright 2018 Embedded Microprocessor Benchmark Consortium (EEMBC)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# 
# Original Author: Shay Gal-on

#File : core_portme.mak

# Flag : OUTFLAG
#	Use this flag to define how to to get an executable (e.g -o)
OUTFLAG= -o
# Flag : CC
#	Use this flag to define compiler to use
CC 		= $(WASI_SDK_ROOT)/bin/clang --sysroot=$(WASI_SDK_ROOT)/share/wasi-sysroot
# Flag : CFLAGS
#	Use this flag to define compiler options. Note, you can add compiler options from the command line using XCFLAGS="other flags"
PORT_CFLAGS = -O3 -nostdlib
FLAGS_STR = "$(PORT_CFLAGS) $(XCFLAGS) $(XLFLAGS) $(LFLAGS_END)"
LFLAGS= -Wl,-z,stack-size=4096 -Wl,--initial-memory=65536 -Wl,--export=main -Wl,--export=__main_argc_argv -Wl,--export=__data_end -Wl,--export=__heap_base -Wl,--strip-all -Wl,--no-entry -Wl,--allow-undefined
CFLAGS = $(PORT_CFLAGS) -I$(PORT_DIR) -I. -DFLAGS_STR=\"$(FLAGS_STR)\" $(LFLAGS)

# Flag : PORT_SRCS
# 	Port specific source files can be added here
#	You may also need cvt.c if the fcvt functions are not provided as intrinsics by your compiler!
PORT_SRCS = $(PORT_DIR)/core_portme.c 
PORT_SRCS += $(PORT_DIR)/ee_printf.c $(PORT_DIR)/cvt.c

PORT_OBJS = $(PORT_DIR)/core_portme.o 
PORT_OBJS += $(PORT_DIR)/ee_printf.o $(PORT_DIR)/cvt.o

# Flag : LOAD
#	For a simple port, we assume self hosted compile and run, no load needed.
LOAD = echo "Please set LOAD to the process of loading the executable to the flash"

# Flag : RUN
#	For a simple port, we assume self hosted compile and run, simple invocation of the executable
RUN = echo "Please set LOAD to the process of running the executable (e.g. via jtag, or board reset)"

OEXT = .o
EXE = -micro.wasm

# Target : port_pre% and port_post%
# For the purpose of this simple port, no pre or post steps needed.

.PHONY : port_prebuild port_postbuild port_prerun port_postrun port_preload port_postload
port_pre% port_post% : 

# FLAG : OPATH
# Path to the output folder. Default - current folder.
OPATH = ./
MKDIR = mkdir -p
