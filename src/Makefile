#
#    Stash, a UCI chess playing engine developed from scratch
#    Copyright (C) 2019-2025 Morgan Houppin
#
#    Stash is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Stash is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

SOURCES := $(wildcard sources/*.c)

OBJECTS := $(SOURCES:%.c=%.o)
DEPENDS := $(SOURCES:%.c=%.d)

ARCH ?=
NATIVE ?= no
ANDROID ?= no
ENGINE_VERSION ?= v37.31
CFLAGS ?= -O3 -flto -DNDEBUG
CPPFLAGS ?= -Werror
LDFLAGS ?=

user_CFLAGS := $(CFLAGS)
user_CPPFLAGS := $(CPPFLAGS)
user_LDFLAGS := $(LDFLAGS)

own_CFLAGS := -std=gnu11
own_CPPFLAGS := -Wall -Wextra -Wcast-qual -Wshadow -Wvla -MMD -I include -DSTASH_UCI_VERSION='"$(ENGINE_VERSION)"'
own_LDFLAGS := -lpthread -lm

# Android's pthread API is provided by bionic's libc, not a separate library.
ifeq ($(ANDROID),yes)
    own_LDFLAGS := -lm
endif

# If no arch is specified, we try to autodetect the target architecture if the
# compiler allows us to do so.

ifeq ($(ARCH),)
	specs := $(shell echo | $(CC) -march=native -E -dM -)
	ifneq ($(findstring __x86_64__,$(specs)),)
		ifneq ($(findstring __BMI2__,$(specs)),)
			# Avoid enabling BMI2 for old Ryzen CPUs who emulate the instruction in
			# microcode.
			ifeq ($(filter __znver1 __znver2,$(specs)),)
				arch:=x86-64-bmi2
			else
				arch:=x86-64-popcnt
			endif
		else
			ifneq ($(findstring __POPCNT__,$(specs)),)
				arch:=x86-64-popcnt
			else
				ifneq ($(findstring __SSE__,$(specs)),)
					arch:=x86-64
				else
					arch:=generic
				endif
			endif
		endif
	else
		arch:=generic
	endif
else
	maybe_arch:=$(filter arm64-dotprod arm64-neon x86-64-bmi2 x86-64-popcnt x86-64-modern x86-64 generic,$(firstword $(ARCH)))

	ifneq ($(maybe_arch),)
		arch:=$(maybe_arch)

		ifeq ($(arch),x86-64-modern)
			_ := $(error Using 'x86-64-modern' as an architecture has been removed since the v36 release. Use 'x86-64-popcnt' instead)
		endif
	else
		arch:=generic
		_ := $(warning Unknown specified architecture '$(ARCH)'. Defaulting to the generic build.)
	endif
endif

# Add .exe to the executable name if we are on Windows, and link the binary statically.

ifeq ($(OS),Windows_NT)
	EXE = stash.exe
	own_LDFLAGS += -static
else
	EXE = stash
endif

# Enable use of PREFETCH instruction

# AArch64 targets. arm64-neon is compatible with ARMv8-A devices; the
# dotprod variant additionally enables the ARMv8.2-A integer dot-product ISA.
# Stash currently has no integer dot-product kernel, so both variants use the
# same NEON bitboard-popcount implementation; the second is an ISA-enabled
# build, not a claim that SDOT is emitted or used by the current engine.
ifeq ($(arch),arm64-neon)
    own_CFLAGS += -march=armv8-a+simd -mtune=cortex-a76
endif

ifeq ($(arch),arm64-dotprod)
    own_CFLAGS += -march=armv8.2-a+dotprod -mtune=cortex-a76
endif

ifeq ($(arch),x86-64)
    ifneq ($(NATIVE),yes)
        own_CFLAGS += -msse
    endif
endif

ifeq ($(arch),x86-64-popcnt)
    ifneq ($(NATIVE),yes)
        own_CFLAGS += -msse -msse3 -mpopcnt
    endif
endif

ifeq ($(arch),x86-64-bmi2)
    own_CFLAGS += -DUSE_PEXT
    ifneq ($(NATIVE),yes)
        own_CFLAGS += -msse -msse3 -mpopcnt -msse4 -mbmi2
    endif
endif

# If native is specified, build will try to use all available CPU instructions

ifeq ($(NATIVE),yes)
    own_CFLAGS += -march=native
endif

override CFLAGS += $(own_CFLAGS)
override CPPFLAGS += $(own_CPPFLAGS)
override LDFLAGS += $(own_LDFLAGS)

all: $(EXE)

$(EXE): $(OBJECTS)
	+$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

-include $(DEPENDS)

clean:
	rm -f $(OBJECTS) $(DEPENDS)

fclean: clean
	rm -f $(EXE)

re:
	$(MAKE) fclean
	+$(MAKE) all CFLAGS="$(user_CFLAGS)" CPPFLAGS="$(user_CPPFLAGS)" LDFLAGS="$(user_LDFLAGS)"

.PHONY: all clean fclean re
