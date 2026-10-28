PROJECT_ROOT := $(abspath ../../..)
SHORT_NAME := G3
FULL_NAME := Gex 3
CONFIG ?= Release
CXX_FLAGS := -std=gnu++98 -fno-exceptions -fno-rtti
APP_SOURCES := $(wildcard $(PROJECT_ROOT)/src/*.c) $(wildcard $(PROJECT_ROOT)/src/*.cpp)
APP_DEFINES := -DWND_TITLE='"Gex 3 (xport)"' -DWND_WIDTH=960 -DWND_HEIGHT=720 -DFIELD_RATE=60
WEB_EXCLUDE_FILES := DATA/AKUJI/* DATA/WARZONE/*

ifeq ($(CONFIG),Release)
APP_DEFINES += -DXPORT_DISABLE_FUNCTION_MARKERS
endif

include ../../../../xport/src/platform/web/Makefile

LINK_FLAGS += -sSTACK_OVERFLOW_CHECK=2 -fno-exceptions
