#
# App
#
F_CONFIG_APP_AUTHOR ?= Unknown
F_CONFIG_APP_DIR ?= untitled
F_CONFIG_APP_NAME ?= Untitled
F_CONFIG_APP_NAME_SUFFIX ?=
F_CONFIG_APP_VERSION_MAJOR ?= 1
F_CONFIG_APP_VERSION_MINOR ?= 0
F_CONFIG_APP_VERSION_MICRO ?= 0

#
# Project dirs
#
F_CONFIG_DIR_ASSETS ?= assets
F_CONFIG_DIR_BUILD ?= build
F_CONFIG_DIR_SCREENSHOTS ?= screenshots
F_CONFIG_DIR_SRC ?= src

#
# Asset file paths
#
F_CONFIG_PATH_EMBED ?=
F_CONFIG_PATH_EMBED_EMSCRIPTEN ?=
F_CONFIG_PATH_GFX ?=
F_CONFIG_PATH_SFX ?=
F_CONFIG_PATH_SRC_GEN ?= 0
F_CONFIG_PATH_STATIC_COPY ?=

#
# Build and compilation options
#
F_CONFIG_BUILD_DEBUG ?= 0
F_CONFIG_BUILD_DEBUG_ALLOC ?= 0
F_CONFIG_BUILD_DEBUG_FATAL_SPIN ?= 0
F_CONFIG_BUILD_DEBUG_WAIT ?= 0
F_CONFIG_BUILD_FLAGS_AR ?=
F_CONFIG_BUILD_FLAGS_C ?=
F_CONFIG_BUILD_FLAGS_C_PEDANTIC ?= 1
F_CONFIG_BUILD_FLAGS_C_STANDARD ?= c99
F_CONFIG_BUILD_FLAGS_CPP ?=
F_CONFIG_BUILD_FLAGS_CPP_PEDANTIC ?= 1
F_CONFIG_BUILD_FLAGS_CPP_STANDARD ?= c++11
F_CONFIG_BUILD_FLAGS_SHARED ?=
F_CONFIG_BUILD_ID ?= default
F_CONFIG_BUILD_LIBS ?=
F_CONFIG_BUILD_MAIN ?= 1
F_CONFIG_BUILD_OPT ?= 0
F_CONFIG_BUILD_PLATFORM ?= unknown
F_CONFIG_BUILD_SRC_EXTRA ?=
F_CONFIG_BUILD_UID := $(F_CONFIG_BUILD_PLATFORM)_$(F_CONFIG_BUILD_ID)

ifdef F_CONFIG_BUILD_SRC_EXTRA
    F_CONFIG_BUILD_SRC_EXTRA := $(realpath $(F_CONFIG_BUILD_SRC_EXTRA))
endif

ifneq ($(F_CONFIG_BUILD_DEBUG_ALLOC), 0)
    F_CONFIG_BUILD_DEBUG := 1
endif

ifneq ($(F_CONFIG_BUILD_DEBUG_WAIT), 0)
    F_CONFIG_BUILD_DEBUG := 1
endif

#
# Systems and platforms
#
F_CONFIG_SYSTEM_ARDUINO ?= 0
F_CONFIG_SYSTEM_CAANOO ?= 0
F_CONFIG_SYSTEM_EMSCRIPTEN ?= 0
F_CONFIG_SYSTEM_GAMEBUINO ?= 0
F_CONFIG_SYSTEM_GP2X ?= 0
F_CONFIG_SYSTEM_LINUX ?= 0
F_CONFIG_SYSTEM_MINGW ?= 0
F_CONFIG_SYSTEM_ODROID_GO ?= 0
F_CONFIG_SYSTEM_PANDORA ?= 0
F_CONFIG_SYSTEM_WIZ ?= 0

F_CONFIG_SYSTEM_GAMEBUINO_NO_EXIT ?= 0

F_CONFIG_SYSTEM_GP2X_MENU ?= 1
F_CONFIG_SYSTEM_GP2X_MHZ ?= 0

F_CONFIG_SYSTEM_EMSCRIPTEN_TOTAL_MEMORY ?=

F_CONFIG_SYSTEM_WIZ_SCREEN_FIX ?= $(F_CONFIG_SYSTEM_WIZ)

#
# System traits
#
F_CONFIG_TRAIT_DESKTOP ?= 0
F_CONFIG_TRAIT_KEYBOARD ?= 0
F_CONFIG_TRAIT_NOSLEEP ?= 0
F_CONFIG_TRAIT_SLOW_MUL ?= 0

#
# Colors
#
F_CONFIG_COLOR_SCREEN_BORDER ?= 0x1f0f0f
F_CONFIG_COLOR_SPRITE_BORDER ?= 0x00FF00
F_CONFIG_COLOR_SPRITE_KEY ?= 0xFF00FF

#
# ECS
#
ifdef F_CONFIG_ECS_COM_NUM
    F_CONFIG_ECS_COM_NUM := $(shell $(FAUR_PATH)/bin/faur-enum -q $(F_DIR_ROOT)/$(F_CONFIG_DIR_SRC)/$(F_CONFIG_ECS_COM_NUM))
else
    F_CONFIG_ECS_COM_NUM := 0
endif

ifdef F_CONFIG_ECS_SYS_NUM
    F_CONFIG_ECS_SYS_NUM := $(shell $(FAUR_PATH)/bin/faur-enum -q $(F_DIR_ROOT)/$(F_CONFIG_DIR_SRC)/$(F_CONFIG_ECS_SYS_NUM))
else
    F_CONFIG_ECS_SYS_NUM := 0
endif

F_CONFIG_ECS_ENABLED := $(shell expr $(F_CONFIG_ECS_COM_NUM) \> 0 \& $(F_CONFIG_ECS_SYS_NUM) \> 0)

#
# Files
#
ifeq ($(F_CONFIG_PATH_EMBED),)
    F_CONFIG_FILES_EMBED ?= 0
else
    F_CONFIG_FILES_EMBED ?= 1
endif

F_CONFIG_FILES_PREFIX ?=
F_CONFIG_FILES_STANDARD ?= 1

#
# Fixed point math
#
F_CONFIG_FIX_LUT ?= 0
F_CONFIG_FIX_LUT_GENERATE ?= 0

#
# FPS
#
F_CONFIG_FPS_CAP_LAG ?= 0
F_CONFIG_FPS_RATE_TICK ?= 30
F_CONFIG_FPS_RATE_DRAW ?= $(F_CONFIG_FPS_RATE_TICK)
F_CONFIG_FPS_HISTORY ?= $(shell expr $(F_CONFIG_FPS_RATE_TICK) \* 2)

ifeq ($(shell expr $(F_CONFIG_FPS_RATE_TICK) \< 1), 1)
    F_CONFIG_FPS_RATE_TICK := 1
endif

ifeq ($(shell expr $(F_CONFIG_FPS_RATE_DRAW) \< 1), 1)
    F_CONFIG_FPS_RATE_DRAW := 1
endif

ifeq ($(shell expr $(F_CONFIG_FPS_RATE_TICK) \< $(F_CONFIG_FPS_RATE_DRAW)), 1)
    F_CONFIG_FPS_RATE_DRAW := F_CONFIG_FPS_RATE_TICK
endif

ifeq ($(shell expr $(F_CONFIG_FPS_HISTORY) \< 1), 1)
    F_CONFIG_FPS_HISTORY := 1
endif

#
# Inputs
#
F_CONFIG_INPUT_MOUSE_CURSOR ?= 1

#
# Make
#
F_CONFIG_MAKE_CLEAN ?=

#
# Debug Output
#
F_CONFIG_CONSOLE_ENABLED ?= 1
F_CONFIG_CONSOLE_SHOW ?= 0

#
# Rendering
#
ifeq ($(F_CONFIG_LIB_SDL), 2)
    F_CONFIG_RENDER ?= SDL2
else
    F_CONFIG_RENDER ?= SOFTWARE
endif

#
# Screen
#
F_CONFIG_SCREEN_HARDWARE_WIDTH ?= 0
F_CONFIG_SCREEN_HARDWARE_HEIGHT ?= 0

ifneq ($(F_CONFIG_SCREEN_HARDWARE_WIDTH), 0)
    F_CONFIG_SCREEN_SIZE_WIDTH ?= $(F_CONFIG_SCREEN_HARDWARE_WIDTH)
endif

ifneq ($(F_CONFIG_SCREEN_HARDWARE_HEIGHT), 0)
    F_CONFIG_SCREEN_SIZE_HEIGHT ?= $(F_CONFIG_SCREEN_HARDWARE_HEIGHT)
endif

ifeq ($(F_CONFIG_LIB_SDL), 1)
    F_CONFIG_SCREEN_FORMAT ?= F_COLOR_FORMAT_ARGB_8888
else ifeq ($(F_CONFIG_LIB_SDL), 2)
    F_CONFIG_SCREEN_FORMAT ?= F_COLOR_FORMAT_ARGB_8888
    F_CONFIG_SCREEN_VSYNC ?= 1
endif

F_CONFIG_SCREEN_FORMAT ?= F_COLOR_FORMAT_RGB_565
F_CONFIG_SCREEN_FULLSCREEN ?= 0
F_CONFIG_SCREEN_MAXIMIZED ?= 1
F_CONFIG_SCREEN_SIZE_WIDTH ?= 320
F_CONFIG_SCREEN_SIZE_HEIGHT ?= 240
F_CONFIG_SCREEN_VSYNC ?= 0
F_CONFIG_SCREEN_ZOOM ?= 1
F_CONFIG_SCREEN_ZOOM_CAN_CHANGE ?= 1

ifneq ($(F_CONFIG_SCREEN_HARDWARE_WIDTH), 0)
    F_CONFIG_SCREEN_ZOOM_CAN_CHANGE := 0
endif

ifneq ($(F_CONFIG_SCREEN_ZOOM), 1)
    F_CONFIG_SYSTEM_WIZ_SCREEN_FIX := 0
endif

#
# Sound
#
F_CONFIG_SOUND_ENABLED ?= 1
F_CONFIG_SOUND_ENABLED_MUSIC ?= 1
F_CONFIG_SOUND_MUTE ?= 0
F_CONFIG_SOUND_SAMPLE_CHANNELS_TOTAL ?= 64
F_CONFIG_SOUND_SAMPLE_CHANNELS_RESERVED ?= 32
F_CONFIG_SOUND_VOLUME_ADJUSTABLE ?= 1
F_CONFIG_SOUND_VOLUME_SCALE_MUSIC ?= 100
F_CONFIG_SOUND_VOLUME_SCALE_SAMPLE ?= 100

ifeq ($(F_CONFIG_SOUND_ENABLED), 0)
    F_CONFIG_SOUND_ENABLED_MUSIC := 0
endif

#
# Libraries
#
F_CONFIG_LIB_PNG ?= 0
F_CONFIG_LIB_SDL ?= 0
F_CONFIG_LIB_SDL_GAMEPADMAP ?= gamecontrollerdb.txt

ifdef F_CONFIG_LIB_SDL_CONFIG
    ifeq ($(F_CONFIG_LIB_SDL), 1)
        ifeq ($(F_CONFIG_SOUND_ENABLED), 1)
            F_CONFIG_BUILD_LIBS += -lSDL_mixer
        endif
    else ifeq ($(F_CONFIG_LIB_SDL), 2)
        ifeq ($(F_CONFIG_SOUND_ENABLED), 1)
            F_CONFIG_BUILD_LIBS += -lSDL2_mixer
        endif
    endif

    F_CONFIG_BUILD_FLAGS_SHARED += $(shell $(F_CONFIG_LIB_SDL_CONFIG) --cflags)
    F_CONFIG_BUILD_LIBS += $(shell $(F_CONFIG_LIB_SDL_CONFIG) --libs)
endif

#
# C defines
#
F_CONFIG_BUILD_FLAGS_SHARED += \
    -DF_CONFIG_APP_AUTHOR=\"$(call F_MAKE_SPACE_ESCAPE,$(F_CONFIG_APP_AUTHOR))\" \
    -DF_CONFIG_APP_DIR=\"$(call F_MAKE_SPACE_ESCAPE,$(F_CONFIG_APP_DIR))\" \
    -DF_CONFIG_APP_NAME=\"$(call F_MAKE_SPACE_ESCAPE,$(F_CONFIG_APP_NAME))\" \
    -DF_CONFIG_APP_VERSION_MAJOR=$(F_CONFIG_APP_VERSION_MAJOR) \
    -DF_CONFIG_APP_VERSION_MICRO=$(F_CONFIG_APP_VERSION_MICRO) \
    -DF_CONFIG_APP_VERSION_MINOR=$(F_CONFIG_APP_VERSION_MINOR) \
    -DF_CONFIG_APP_VERSION_STRING=\"$(F_CONFIG_APP_VERSION_MAJOR).$(F_CONFIG_APP_VERSION_MINOR).$(F_CONFIG_APP_VERSION_MICRO)\" \
    -DF_CONFIG_BUILD_DEBUG=$(F_CONFIG_BUILD_DEBUG) \
    -DF_CONFIG_BUILD_DEBUG_ALLOC=$(F_CONFIG_BUILD_DEBUG_ALLOC) \
    -DF_CONFIG_BUILD_DEBUG_FATAL_SPIN=$(F_CONFIG_BUILD_DEBUG_FATAL_SPIN) \
    -DF_CONFIG_BUILD_DEBUG_WAIT=$(F_CONFIG_BUILD_DEBUG_WAIT) \
    -DF_CONFIG_BUILD_MAIN=$(F_CONFIG_BUILD_MAIN) \
    -DF_CONFIG_BUILD_UID=\"$(F_CONFIG_BUILD_UID)\" \
    -DF_CONFIG_COLOR_SPRITE_BORDER=$(F_CONFIG_COLOR_SPRITE_BORDER) \
    -DF_CONFIG_COLOR_SPRITE_KEY=$(F_CONFIG_COLOR_SPRITE_KEY) \
    -DF_CONFIG_COLOR_SCREEN_BORDER=$(F_CONFIG_COLOR_SCREEN_BORDER) \
    -DF_CONFIG_CONSOLE_ENABLED=$(F_CONFIG_CONSOLE_ENABLED) \
    -DF_CONFIG_CONSOLE_SHOW=$(F_CONFIG_CONSOLE_SHOW) \
    -DF_CONFIG_DIR_SCREENSHOTS=\"$(F_CONFIG_DIR_SCREENSHOTS)\" \
    -DF_CONFIG_ECS_COM_NUM=$(F_CONFIG_ECS_COM_NUM) \
    -DF_CONFIG_ECS_ENABLED=$(F_CONFIG_ECS_ENABLED) \
    -DF_CONFIG_ECS_SYS_NUM=$(F_CONFIG_ECS_SYS_NUM) \
    -DF_CONFIG_FILES_EMBED=$(F_CONFIG_FILES_EMBED) \
    -DF_CONFIG_FILES_PREFIX=\"$(F_CONFIG_FILES_PREFIX)\" \
    -DF_CONFIG_FILES_STANDARD=$(F_CONFIG_FILES_STANDARD) \
    -DF_CONFIG_FIX_LUT=$(F_CONFIG_FIX_LUT) \
    -DF_CONFIG_FIX_LUT_GENERATE=$(F_CONFIG_FIX_LUT_GENERATE) \
    -DF_CONFIG_FPS_CAP_LAG=$(F_CONFIG_FPS_CAP_LAG) \
    -DF_CONFIG_FPS_HISTORY=$(F_CONFIG_FPS_HISTORY) \
    -DF_CONFIG_FPS_RATE_DRAW=$(F_CONFIG_FPS_RATE_DRAW) \
    -DF_CONFIG_FPS_RATE_TICK=$(F_CONFIG_FPS_RATE_TICK) \
    -DF_CONFIG_INPUT_MOUSE_CURSOR=$(F_CONFIG_INPUT_MOUSE_CURSOR) \
    -DF_CONFIG_LIB_PNG=$(F_CONFIG_LIB_PNG) \
    -DF_CONFIG_LIB_SDL=$(F_CONFIG_LIB_SDL) \
    -DF_CONFIG_LIB_SDL_GAMEPADMAP=\"$(F_CONFIG_LIB_SDL_GAMEPADMAP)\" \
    -DF_CONFIG_RENDER_$(F_CONFIG_RENDER)=1 \
    -DF_CONFIG_SCREEN_FORMAT=$(F_CONFIG_SCREEN_FORMAT) \
    -DF_CONFIG_SCREEN_FULLSCREEN=$(F_CONFIG_SCREEN_FULLSCREEN) \
    -DF_CONFIG_SCREEN_HARDWARE_HEIGHT=$(F_CONFIG_SCREEN_HARDWARE_HEIGHT) \
    -DF_CONFIG_SCREEN_HARDWARE_WIDTH=$(F_CONFIG_SCREEN_HARDWARE_WIDTH) \
    -DF_CONFIG_SCREEN_MAXIMIZED=$(F_CONFIG_SCREEN_MAXIMIZED) \
    -DF_CONFIG_SCREEN_SIZE_HEIGHT=$(F_CONFIG_SCREEN_SIZE_HEIGHT) \
    -DF_CONFIG_SCREEN_SIZE_WIDTH=$(F_CONFIG_SCREEN_SIZE_WIDTH) \
    -DF_CONFIG_SCREEN_VSYNC=$(F_CONFIG_SCREEN_VSYNC) \
    -DF_CONFIG_SCREEN_ZOOM=$(F_CONFIG_SCREEN_ZOOM) \
    -DF_CONFIG_SCREEN_ZOOM_CAN_CHANGE=$(F_CONFIG_SCREEN_ZOOM_CAN_CHANGE) \
    -DF_CONFIG_SOUND_ENABLED=$(F_CONFIG_SOUND_ENABLED) \
    -DF_CONFIG_SOUND_ENABLED_MUSIC=$(F_CONFIG_SOUND_ENABLED_MUSIC) \
    -DF_CONFIG_SOUND_MUTE=$(F_CONFIG_SOUND_MUTE) \
    -DF_CONFIG_SOUND_SAMPLE_CHANNELS_RESERVED=$(F_CONFIG_SOUND_SAMPLE_CHANNELS_RESERVED) \
    -DF_CONFIG_SOUND_SAMPLE_CHANNELS_TOTAL=$(F_CONFIG_SOUND_SAMPLE_CHANNELS_TOTAL) \
    -DF_CONFIG_SOUND_VOLUME_ADJUSTABLE=$(F_CONFIG_SOUND_VOLUME_ADJUSTABLE) \
    -DF_CONFIG_SOUND_VOLUME_SCALE_MUSIC=$(F_CONFIG_SOUND_VOLUME_SCALE_MUSIC) \
    -DF_CONFIG_SOUND_VOLUME_SCALE_SAMPLE=$(F_CONFIG_SOUND_VOLUME_SCALE_SAMPLE) \
    -DF_CONFIG_SYSTEM_ARDUINO=$(F_CONFIG_SYSTEM_ARDUINO) \
    -DF_CONFIG_SYSTEM_CAANOO=$(F_CONFIG_SYSTEM_CAANOO) \
    -DF_CONFIG_SYSTEM_EMSCRIPTEN=$(F_CONFIG_SYSTEM_EMSCRIPTEN) \
    -DF_CONFIG_SYSTEM_GAMEBUINO=$(F_CONFIG_SYSTEM_GAMEBUINO) \
    -DF_CONFIG_SYSTEM_GAMEBUINO_NO_EXIT=$(F_CONFIG_SYSTEM_GAMEBUINO_NO_EXIT) \
    -DF_CONFIG_SYSTEM_GP2X=$(F_CONFIG_SYSTEM_GP2X) \
    -DF_CONFIG_SYSTEM_GP2X_MENU=$(F_CONFIG_SYSTEM_GP2X_MENU) \
    -DF_CONFIG_SYSTEM_GP2X_MHZ=$(F_CONFIG_SYSTEM_GP2X_MHZ) \
    -DF_CONFIG_SYSTEM_LINUX=$(F_CONFIG_SYSTEM_LINUX) \
    -DF_CONFIG_SYSTEM_MINGW=$(F_CONFIG_SYSTEM_MINGW) \
    -DF_CONFIG_SYSTEM_ODROID_GO=$(F_CONFIG_SYSTEM_ODROID_GO) \
    -DF_CONFIG_SYSTEM_PANDORA=$(F_CONFIG_SYSTEM_PANDORA) \
    -DF_CONFIG_SYSTEM_WIZ=$(F_CONFIG_SYSTEM_WIZ) \
    -DF_CONFIG_SYSTEM_WIZ_SCREEN_FIX=$(F_CONFIG_SYSTEM_WIZ_SCREEN_FIX) \
    -DF_CONFIG_TRAIT_DESKTOP=$(F_CONFIG_TRAIT_DESKTOP) \
    -DF_CONFIG_TRAIT_KEYBOARD=$(F_CONFIG_TRAIT_KEYBOARD) \
    -DF_CONFIG_TRAIT_NOSLEEP=$(F_CONFIG_TRAIT_NOSLEEP) \
    -DF_CONFIG_TRAIT_SLOW_MUL=$(F_CONFIG_TRAIT_SLOW_MUL) \
