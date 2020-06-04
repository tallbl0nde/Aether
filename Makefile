ifndef DEVKITPRO
$(error DEVKITPRO is not present in your environment. This can be fixed by sourcing switchvars.sh from /opt/devkitpro/)
endif

include $(DEVKITPRO)/devkitA64/base_tools

BUILD	:=	build
TARGET	:=  Aether
SOURCE	:=	source
INCLUDE	:=	include

DOCS_CONFIG	:=	Doxyfile

OUTPUT		:=	lib/lib$(TARGET).a
ARCH		:=	-march=armv8-a -mtune=cortex-a57 -mtp=soft -fPIC -ftls-model=local-exec
DEFINES		:=	-D__SWITCH__
LIBDIRS		:=	$(PORTLIBS) $(LIBNX)
INCLUDE		:=	$(foreach dir,$(LIBDIRS),-I$(dir)/include) -I$(INCLUDE)
CFLAGS		:=	-w -O3 -ffunction-sections -fdata-sections $(shell sdl2-config --cflags)\
				$(shell freetype-config --cflags) $(ARCH) $(INCLUDE) $(DEFINES)
CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++17
LIBS		:=	-lstdc++fs -lnx $(shell sdl2-config --libs) -lSDL2_ttf -lSDL2_gfx -lSDL2_image\
				$(shell freetype-config --libs) -lpng -ljpeg -lwebp
CFILES		:=	$(wildcard $(SOURCE)/*.c) $(wildcard $(SOURCE)/*/*.c)
CPPFILES	:=	$(wildcard $(SOURCE)/*.cpp) $(wildcard $(SOURCE)/*/*.cpp)
OBJSDIR		:=	$(BUILD)/objs
DEPSDIR		:=	$(BUILD)/deps
OFILES		:=	$(foreach CPPFILE,$(CPPFILES),$(patsubst $(SOURCE)/%.cpp,$(OBJSDIR)/%.o,$(CPPFILE)))\
				$(foreach CFILE,$(CFILES),$(patsubst $(SOURCE)/%.c,$(OBJSDIR)/%.o,$(CFILE)))
DEPENDS		:=	$(foreach OFILE,$(OFILES),$(patsubst $(OBJSDIR)/%.o,$(DEPSDIR)/%.d,$(OFILE)))

-include $(DEPENDS)
.PHONY: $(BUILD) library install uninstall docs clean cleandocs

library : $(OUTPUT)
install : library
$(OUTPUT) : $(OFILES)
$(OFILES) : $(OBJSDIR) $(DEPSDIR)
$(OBJSDIR) : $(BUILD)
$(DEPSDIR) : $(BUILD)

$(BUILD):
	@echo Creating build directory if it doesn\'t exist...
	@[ -d $@ ] || mkdir -p $@
	@echo Creating output lib directory if it doesn\'t exist...
	@[ -d $(CURDIR)/lib ] || mkdir -p $(CURDIR)/lib

$(OBJSDIR):
	@echo Creating object directories if it doesn\'t exist...
	@[ -d $@ ] || mkdir -p $@
	@[ -d $@/base ] || mkdir -p $@/base
	@[ -d $@/horizon ] || mkdir -p $@/horizon
	@[ -d $@/primary ] || mkdir -p $@/primary
	@[ -d $@/utils ] || mkdir -p $@/utils

$(DEPSDIR):
	@echo Creating object dependent directories if it doesn\'t exist...
	@[ -d $@ ] || mkdir -p $@
	@[ -d $@/base ] || mkdir -p $@/base
	@[ -d $@/horizon ] || mkdir -p $@/horizon
	@[ -d $@/primary ] || mkdir -p $@/primary
	@[ -d $@/utils ] || mkdir -p $@/utils

clean:
	@echo -n Cleaning build files...
	@rm -fr $(OUTPUT) $(OBJSDIR)/*.o $(DEPSDIR)/*.d $(OBJSDIR)/*/*.o $(DEPSDIR)/*/*.d
	@echo Done!

cleandocs:
	@echo -n Cleaning generated doc files...
	@rm -fr docs/*
	@echo Done!

install:
	@echo -n Installing Aether headers files...
	@mkdir -p $(PORTLIBS_PREFIX)/include/Aether/
	@cp -r $(INCLUDES)/Aether/* $(PORTLIBS_PREFIX)/include/Aether/
	@echo Done!
	@echo -n Installing Aether library files...
	@mkdir -p $(PORTLIBS_PREFIX)/lib/
	@cp $(OUTPUT) $(PORTLIBS_PREFIX)/lib/
	@echo Done!

uninstall:
	@echo -n Un-installing Aether headers files if it is installed...
	@rm -fr $(PORTLIBS_PREFIX)/include/Aether/
	@echo Done!
	@echo -n Un-installing Aether library files if it is installed...
	@rm $(PORTLIBS_PREFIX)/lib/libAether.a
	@echo Done!

$(OUTPUT):
	@rm -rf $(OUTPUT)
	@echo -n Creating Aether library archive at $(OUTPUT)...
	@$(AR) -rc $(OUTPUT) $(OFILES)
	@echo Done!

docs:
	@echo Creating docs directory if it doesn't exist
	@[ -d $@ ] || mkdir -p $@
	@echo -n Generating docs
	@doxygen $(DOCS_CONFIG)
	@echo Done!

$(OBJSDIR)/%.o: $(SOURCE)/%.cpp
	@echo -n Creating object $@...
	@$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) -c $< -o $@ $(ERROR_FILTER)
	@echo Done!

$(OBJSDIR)/%.o: $(SOURCE)/%.c
	@echo -n Creating object $@...
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) -c $< -o $@ $(ERROR_FILTER)
	@echo Done!