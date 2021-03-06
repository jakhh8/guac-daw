# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

RESCOMP = windres
TARGETDIR = ../bin/Debug-linux-x86_64/guac-daw
TARGET = $(TARGETDIR)/guac-daw
OBJDIR = ../bin-int/Debug-linux-x86_64/guac-daw
DEFINES += -DGM_DEBUG
INCLUDES += -Iguac-daw/src -I../Guacamole/vendor/spdlog/include -I../Guacamole/src -I../Guacamole/vendor -I../Guacamole/vendor/imgui -I../Guacamole/vendor/glm -I../Guacamole/vendor/EnTT -I../Guacamole/vendor/yaml-cpp/include -I../Guacamole/vendor/ImGuizmo -I../SoundApp/vendor/openal-soft/include/AL -Ivendor/openal-soft/common
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -g -std=c++17
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LIBS += ../bin/Debug-linux-x86_64/Guacamole/libGuacamole.a ../Guacamole/vendor/GLFW/bin/Debug-linux-x86_64/GLFW/libGLFW.a ../Guacamole/vendor/Glad/bin/Debug-linux-x86_64/Glad/libGlad.a ../Guacamole/vendor/imgui/bin/Debug-linux-x86_64/ImGui/libImGui.a ../Guacamole/vendor/yaml-cpp/bin/Debug-linux-x86_64/yaml-cpp/libyaml-cpp.a -lopenal -lpthread -ldl
LDDEPS += ../bin/Debug-linux-x86_64/Guacamole/libGuacamole.a ../Guacamole/vendor/GLFW/bin/Debug-linux-x86_64/GLFW/libGLFW.a ../Guacamole/vendor/Glad/bin/Debug-linux-x86_64/Glad/libGlad.a ../Guacamole/vendor/imgui/bin/Debug-linux-x86_64/ImGui/libImGui.a ../Guacamole/vendor/yaml-cpp/bin/Debug-linux-x86_64/yaml-cpp/libyaml-cpp.a
ALL_LDFLAGS += $(LDFLAGS) -Lvendor/openal-soft/build -L/usr/lib64 -m64
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/DawLayer.o
GENERATED += $(OBJDIR)/GuacDawApp.o
GENERATED += $(OBJDIR)/SequencerPanel.o
GENERATED += $(OBJDIR)/main.o
OBJECTS += $(OBJDIR)/DawLayer.o
OBJECTS += $(OBJDIR)/GuacDawApp.o
OBJECTS += $(OBJDIR)/SequencerPanel.o
OBJECTS += $(OBJDIR)/main.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking guac-daw
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning guac-daw
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) rmdir /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/DawLayer.o: src/DawLayer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GuacDawApp.o: src/GuacDawApp.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/SequencerPanel.o: src/Panels/SequencerPanel.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/main.o: src/main.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif