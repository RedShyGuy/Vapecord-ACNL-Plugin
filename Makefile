#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

export TOPDIR ?= $(CURDIR)
include $(DEVKITARM)/3ds_rules

CTRPFLIB	?=	$(DEVKITPRO)/libctrpfforvapecord

TARGET		:= 	Vapecord_Public

BUILD		:= 	Build
INCLUDES	:= 	include \
				include/core \
				include/platform/ctrpf \
				include/core/game_api \
				include/core/game_api/House \
				include/core/hooks \
				include/core/checks \
				include/core/infrastructure \
				include/core/Pretendo \
				include/core/libgarden \
				include/core/libgarden/garden \
				include/core/libgarden/garden/ac \
				include/core/libgarden/garden/applet \
				include/core/libgarden/garden/audio \
				include/core/libgarden/garden/bs \
				include/core/libgarden/garden/camera \
				include/core/libgarden/garden/controller \
				include/core/libgarden/garden/demo \
				include/core/libgarden/garden/fgobj \
				include/core/libgarden/garden/field \
				include/core/libgarden/garden/font \
				include/core/libgarden/garden/framework \
				include/core/libgarden/garden/g3d \
				include/core/libgarden/garden/human \
				include/core/libgarden/garden/island \
				include/core/libgarden/garden/item \
				include/core/libgarden/garden/math \
				include/core/libgarden/garden/net \
				include/core/libgarden/garden/netgame \
				include/core/libgarden/garden/npc \
				include/core/libgarden/garden/oml \
				include/core/libgarden/garden/player \
				include/core/libgarden/garden/proc \
				include/core/libgarden/garden/sc \
				include/core/libgarden/garden/script \
				include/core/libgarden/garden/sound \
				include/core/libgarden/garden/ssys \
				include/core/libgarden/garden/stage \
				include/core/libgarden/garden/sv \
				include/core/libgarden/garden/tour \
				include/core/libgarden/gardenex \
				include/core/libgarden/gardenex/bs \
				include/core/libgarden/gardenex/detail \
				include/core/libgarden/gardenex/field \
				include/core/libgarden/gardenex/script \
				include/core/libgarden/gardenex/state \
				include/core/libgarden/gardenex/util \
				include/core/libgarden/GLES2 \
				include/core/libgarden/nn \
				include/core/libgarden/nn/gx \
				include/core/libgarden/nn/math \
				include/core/libgarden/nn/os \
				include/core/libgarden/nn/pia \
				include/core/libgarden/nn/pia/common \
				include/core/libgarden/nn/pia/inet \
				include/core/libgarden/nn/pia/session \
				include/core/libgarden/nn/pia/transport \
				include/core/libgarden/nw \
				include/core/libgarden/nw/font \
				include/core/libgarden/nw/gfx \
				include/core/libgarden/nw/lyt \
				include/core/libgarden/nw/os \
				include/core/libgarden/nw/ut \
				include/core/libgarden/sead \
				include/core/libgarden/sead/math \

SOURCES 	:= 	src \
                src/features \
				src/features/SeedingCodes \
				src/features/ExtraCodes \
				src/features/PlayerCodes \
				src/features/DefaultCodes \
				src/features/EnvironmentCodes \
				src/features/MiniGame1 \
				src/features/InventoryCodes \
				src/core \
				src/core/game_api \
				src/core/game_api/House \
				src/core/hooks \
				src/core/checks \
				src/core/infrastructure \
				src/core/Pretendo \
				src/platform/ctrpf \
				src/platform \

PSF 		:= 	$(notdir $(TOPDIR)).plgInfo

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
DEVMODE 	?= 1

ARCH		:=	-march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

FALSEPOSITIVES := -Wno-array-bounds -Wno-stringop-overflow -Wno-stringop-overread

CFLAGS		:=	$(ARCH) -Os -mword-relocations \
				-fomit-frame-pointer -ffunction-sections -fno-strict-aliasing \
				$(FALSEPOSITIVES)

CFLAGS		+=	$(INCLUDE) -D__3DS__

CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++23 -DDEVMODE=$(DEVMODE)

ASFLAGS		:=	$(ARCH)
LDFLAGS		:= -T $(TOPDIR)/3gx.ld $(ARCH) -Os -Wl,--gc-sections,--strip-discarded,--strip-debug

LIBS		:= -lctrpf -lctru
LIBDIRS		:= 	$(CTRPFLIB) $(CTRULIB) $(PORTLIBS)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES			:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES			:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

export LD 		:= 	$(CXX)
export OFILES	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I $(CURDIR)/$(dir) ) \
					$(foreach dir,$(LIBDIRS),-I $(dir)/include) \
					-I $(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L $(dir)/lib)

.PHONY: $(BUILD) clean all check-address

#---------------------------------------------------------------------------------
all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT).3gx $(OUTPUT).elf

re: clean all

#---------------------------------------------------------------------------------

relink:
	@rm -f *.elf *.3gx
	@$(MAKE)

check-address:
	@python3 $(CURDIR)/tools/check_no_global_address.py || python $(CURDIR)/tools/check_no_global_address.py

#---------------------------------------------------------------------------------

else

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------

DEPENDS	:=	$(OFILES:.o=.d)


$(OUTPUT).3gx : $(OUTPUT).elf

$(OUTPUT).elf : $(OFILES)
#---------------------------------------------------------------------------------
# you need a rule like this for each extension you use as binary data
#---------------------------------------------------------------------------------
%.bin.o	:	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.3gx: %.elf
	@echo creating $(notdir $@)
	@3gxtool -s -d $^ $(TOPDIR)/$(PSF) $@

-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
