.SUFFIXES:

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

TOPDIR 		?= 	$(CURDIR)
include $(DEVKITARM)/3ds_rules

CTRPFLIB	?=	$(CURDIR)/libctrpf/Library

TARGET		:= 	$(notdir $(CURDIR))
PLGINFO 	:= 	CTRPluginFramework.plgInfo

BUILD		:= 	Build
INCLUDES	:= 	Includes
SOURCES 	:= 	Sources \
                Sources/Folders \
				Sources/Folders/SeedingCodes \
				Sources/Folders/ExtraCodes \
				Sources/Folders/PlayerCodes \
				Sources/Helpers \
				Sources/Helpers/Personal \
				Sources/Helpers/Other \
				Sources/Plugin
				
#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH		:=	-march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

FALSEPOSITIVES := -Wno-array-bounds -Wno-stringop-overflow -Wno-stringop-overread

CFLAGS		:=	$(ARCH) -Os -mword-relocations \
				-fomit-frame-pointer -ffunction-sections -fno-strict-aliasing \
				$(FALSEPOSITIVES)

CFLAGS		+=	$(INCLUDE) -D__3DS__

CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++17

ASFLAGS		:=	$(ARCH)
LDFLAGS		:= -T $(TOPDIR)/3gx.ld $(ARCH) -Os -Wl,--gc-sections,--strip-discarded,--strip-debug

LDFLAGS += -L/$(CURDIR)/libctrpf/Library/lib
CXXFLAGS += -I/$(CURDIR)/libctrpf/Library/include

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

.PHONY: $(BUILD) clean all

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

else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
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
.PRECIOUS: %.elf
%.3gx: %.elf
#---------------------------------------------------------------------------------
	@echo creating $(notdir $@)
	@3gxtool -s -d $(word 1, $^) $(TOPDIR)/$(PLGINFO) $@

-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
