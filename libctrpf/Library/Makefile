#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/3ds_rules

export CTRPF_REVISION		:=	$(shell printf "%s.r%s" "$$(git describe --abbrev=0)" "$$(git rev-list --count HEAD)")
export CTRPF_VERSION_MAJOR	:=	$(shell git describe --always --tags | cut -f1 -d- | cut -f1 -d.)
export CTRPF_VERSION_MINOR	:=	$(shell git describe --always --tags | cut -f1 -d- | cut -f2 -d.)
export CTRPF_VERSION_BUILD	:=	$(shell git describe --always --tags | cut -f1 -d- | cut -f3 -d.)
export COMMIT			:=	$(shell git rev-parse --short=8 HEAD)
export COMPILE_DATE 	:=  $(shell date -u +"%Y-%m-%dT%H:%M:%S%Z")

ENABLE_LINK_TIME_OPTIMIZATIONS = 0

#ifeq ($(strip $(CTRPF_REVISION)),)
#	export CTRPF_REVISION			:=	v0.0.0-0
#	export CTRPF_VERSION_MAJOR	:=	0
#	export CTRPF_VERSION_MINOR	:=	0
#	export CTRPF_VERSION_BUILD	:=	0
#endif

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# DATA is a list of directories containing data files
# INCLUDES is a list of directories containing header files
#---------------------------------------------------------------------------------
TARGET		:=	ctrpf
#BUILD		:=	build
SOURCES		:=	source \
				source/CTRPluginFramework \
				source/CTRPluginFramework/Graphics \
				source/CTRPluginFramework/Menu \
				source/CTRPluginFramework/Sound \
				source/CTRPluginFramework/System \
				source/CTRPluginFramework/Utils \
				source/CTRPluginFrameworkImpl \
				source/CTRPluginFrameworkImpl/ActionReplay \
				source/CTRPluginFrameworkImpl/Disassembler \
				source/CTRPluginFrameworkImpl/Graphics \
				source/CTRPluginFrameworkImpl/Graphics/Icons \
				source/CTRPluginFrameworkImpl/Menu \
				source/CTRPluginFrameworkImpl/Search \
				source/CTRPluginFrameworkImpl/Sound \
				source/CTRPluginFrameworkImpl/System \
				source/CTRPluginFrameworkImpl/System/Services \
				source/ctrulibExtension \
				source/ctrulibExtension/allocator \
				source/ctrulibExtension/gpu \
				source/ctrulibExtension/services \
				source/ctrulibExtension/system \
				source/ctrulibExtension/util/utf \
				source/ctrulibExtension/util/rbtree \
				source/lodepng


#DATA		:=	data
INCLUDES	:=	include

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

DEFINES	:=	-DCTRPF_VERSION_MAJOR="\"$(CTRPF_VERSION_MAJOR)\"" -DCTRPF_VERSION_MINOR="\"$(CTRPF_VERSION_MINOR)\"" \
			-DCTRPF_VERSION_BUILD="\"$(CTRPF_VERSION_BUILD)\"" -DCOMMIT_HASH="\"$(COMMIT)\"" -DCOMPILE_DATE="\"$(COMPILE_DATE)\""

GCC11_FALSEPOSITIVES := -Wno-array-bounds -Wno-stringop-overflow -Wno-stringop-overread
CFLAGS	:=	-Wall -Werror -Wno-psabi -mword-relocations \
			-ffunction-sections \
			-fdata-sections \
			$(GCC11_FALSEPOSITIVES) \
			$(ARCH) \
			$(BUILD_CFLAGS) $(G)

CFLAGS	+=	$(INCLUDE) -D__3DS__ $(DEFINES)

ASFLAGS	:=	$(G) $(ARCH) $(INCLUDE) -Os -Wl,$(WL)--gc-sections

ifneq ($(ENABLE_LINK_TIME_OPTIMIZATIONS), 0)
	CFLAGS += -flto=auto -ffat-lto-objects
	ASFLAGS += -flto=auto -ffat-lto-objects
endif

CXXFLAGS 	:= $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++20

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBS	:= -lctru -lm -lcwav -lncsnd
LIBDIRS	:= $(CTRULIB) $(CURDIR)/libcwav $(CURDIR)/libcwav/libncsnd

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir))

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
#BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES_BIN	:=	#$(addsuffix .o,$(BINFILES))
export OFILES_SRC	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export HFILES_BIN	:=	$(addsuffix .h,$(subst .,_,$(BINFILES)))

export OFILES	:=	$(OFILES_BIN) $(OFILES_SRC)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I.

.PHONY: clean all

#---------------------------------------------------------------------------------
all: lib/libctrpf.a lib/libctrpfd.a

dist-bin: all
	@tar --exclude=*~ -cjf libctrpf-$(CTRPF_REVISION).tar.bz2 include/CTRPluginFramework include/CTRPluginFramework.hpp lib

dist-src:
	@tar --exclude=*~ -cjf libctru-src-$(CTRPF_REVISION).tar.bz2 include source data Makefile Doxyfile

dist: dist-bin #dist-src

install: dist
	@rm -rf $(DESTDIR)$(DEVKITPRO)/libctrpf
	@mkdir -p $(DESTDIR)$(DEVKITPRO)/libctrpf
	@bzip2 -cd libctrpf-$(CTRPF_REVISION).tar.bz2 | tar -xf - -C $(DESTDIR)$(DEVKITPRO)/libctrpf
	@echo "CTRPF was successfully installed in $(DESTDIR)$(DEVKITPRO)/libctrpf"

libcwav:
	@git clone --recursive https://github.com/mariohackandglitch/libcwav.git $@

libcwav-update: libcwav
	@cd libcwav && git pull

libcwav-build: libcwav-update
	@cd libcwav/libncsnd && $(MAKE)
	@cd libcwav && CFLAGS='-DCWAV_DISABLE_DSP' $(MAKE)

lib:
	@[ -d $@ ] || mkdir -p $@

release:
	@[ -d $@ ] || mkdir -p $@

debug:
	@[ -d $@ ] || mkdir -p $@

lib/libctrpf.a : SHELL := /bin/bash
lib/libctrpf.a : libcwav-build lib release
	@$(MAKE) BUILD=release OUTPUT=$(CURDIR)/$@ WL=--strip-discarded,--strip-debug, \
	BUILD_CFLAGS="-DNDEBUG=1 -Os -fomit-frame-pointer" DEPSDIR=$(CURDIR)/release \
	--no-print-directory -C release	-f $(CURDIR)/Makefile
	@rm release/PluginMenuTools.o # Force update of revision, date etc
	@cp $(CURDIR)/libcwav/lib/libcwav.a lib/libcwav.a
	@cp $(CURDIR)/libcwav/libncsnd/lib/libncsnd.a lib/libncsnd.a
	@mv $@ $@tmp
	@$(AR) -M <<< $$'create $@ \naddlib $@tmp \naddlib lib/libcwav.a\naddlib lib/libncsnd.a\nsave\nend'
	@rm $@tmp
	@rm lib/libcwav.a
	@rm lib/libncsnd.a

lib/libctrpfd.a : SHELL := /bin/bash
lib/libctrpfd.a : libcwav-build lib debug
	@$(MAKE) BUILD=debug OUTPUT=$(CURDIR)/$@ BUILD_CFLAGS="-DDEBUG=1" G=-g \
	DEPSDIR=$(CURDIR)/debug --no-print-directory -C debug -f $(CURDIR)/Makefile
	@rm debug/PluginMenuTools.o # Force update of revision, date etc.
	@cp $(CURDIR)/libcwav/lib/libcwav.a lib/libcwav.a
	@cp $(CURDIR)/libcwav/libncsnd/lib/libncsnd.a lib/libncsnd.a
	@mv $@ $@tmp
	@$(AR) -M <<< $$'create $@ \naddlib $@tmp \naddlib lib/libcwav.a\naddlib lib/libncsnd.a\nsave\nend'
	@rm $@tmp
	@rm lib/libcwav.a
	@rm lib/libncsnd.a

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr release debug lib docs libcwav

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT)	:	$(OFILES)

$(OFILES_SRC) : $(HFILES_BIN)

#---------------------------------------------------------------------------------
%.bin.o	%_bin.h :	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)


-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------