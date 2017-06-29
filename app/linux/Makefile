PLATFORM_ARCH := $(shell uname -m)
ifndef ARCH
  MAKEFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
  MAKEFILE_DIR := $(abspath $(dir $(MAKEFILE)))
  # Makefile is currently stored in Master_Thesis/app/linux
  PRJ_ROOT := $(abspath $(MAKEFILE_DIR))
  OPENCV_PATH := usr/local
  ARCH := $(PLATFORM_ARCH)
  CXX_$(ARCH) := g++
  CXXFLAGS_$(ARCH) :=
  LDFLAGS_$(ARCH) :=
else
  ifneq ($(filter undefined,$(foreach tool,CXX,$(origin $(tool)_$(ARCH)))),)
    $(error Cannot cross-compile for target platform $(ARCH) on this $(PLATFORM_ARCH) platform)
  endif
endif

################################################################################

BIN_NAME := DistanceComparer

BUILD_ARCH := $(ARCH)

SRC := ../_sources
BIN := ../../bin/linux
BUILD := ../../.build/linux

BIN_DIR := $(BIN)/$(BUILD_ARCH)/$(BUILD_TYPE)
OBJ_DIR := $(BUILD)/$(BUILD_ARCH)/$(BUILD_TYPE)

OBJ_SUBDIRS := $(SRC)

################################################################################


DEPS := $(sort $(wildcard $(SRC)/*.h))
SRCS := $(sort $(wildcard $(SRC)/*.cpp))
OBJS := $(sort $(patsubst $(SRC)/%.cpp,$(OBJ_DIR)/%.opp,$(SRCS)))

################################################################################


CXX := $(CXX_$(ARCH))

WARNS := -Wall
WARNS += -Wextra
#WARNS += -Wconversion
WARNS += -Wno-unused-variable -Wno-unused-parameter \
         -Wno-unused-but-set-variable -Wno-unknown-pragmas
WARNS += -Werror=implicit-function-declaration


CXXFLAGS = $(WARNS)
CXXFLAGS += $(CXXFLAGS_EXTRA)
CXXFLAGS += $(CXXFLAGS_$(ARCH))
CXXFLAGS += -std=c++11
CXXFLAGS += -I$(SRC)
CXXFLAGS += -I$(OPENCV_PATH)/include


LDFLAGS := -ldl -lpthread
# Enable compilation on older systems
LDFLAGS += -lrt
LDFLAGS += $(LDFLAGS_$(ARCH))
LDFLAGS += -lopencv_highgui 
LDFLAGS += -lopencv_imgproc 
LDFLAGS += -lopencv_core
LDFLAGS += -lopencv_features2d
LDFLAGS += -lopencv_flann
LDFLAGS += -lopencv_imgcodecs
LDFLAGS += -lopencv_ml
LDFLAGS += -lopencv_objdetect
LDFLAGS += -lopencv_photo
LDFLAGS += -lopencv_shape
LDFLAGS += -lopencv_stitching
LDFLAGS += -lopencv_superres

LDFLAGS += -Wl,-rpath=/$(OPENCV_PATH)/lib

CXXFLAGS_DEBUG := -g -O0
CXXFLAGS_DEBUG += -DDEBUG -D_DEBUG

CXXFLAGS_RELEASE := -O3
CXXFLAGS_RELEASE += -DNDEBUG

################################################################################
# THOSE ARE THE TARGETS USABLE FROM COMMAND LINE

.PHONY: default
default: | all

.PHONY: all
all: | debug release

.PHONY: clean
clean: | clean_debug clean_release

.PHONY: distbuild
distbuild: | all

.PHONY: distclean
distclean: | distclean_debug distclean_release

#####

.PHONY: debug clean_debug distclean_debug
debug clean_debug distclean_debug: export BUILD_TYPE := debug

.PHONY: release clean_release distclean_release
release clean_release distclean_release: export BUILD_TYPE := release

debug: export CXXFLAGS_EXTRA := $(CXXFLAGS_DEBUG)

release: export CXXFLAGS_EXTRA := $(CXXFLAGS_RELEASE)

debug release:
	$(MAKE) -C $(PRJ_ROOT) --no-print-directory -f $(MAKEFILE) _app

clean_debug clean_release:
	$(MAKE) -C $(PRJ_ROOT) --no-print-directory -f $(MAKEFILE) _clean

distclean_debug distclean_release:
	$(MAKE) -C $(PRJ_ROOT) --no-print-directory -f $(MAKEFILE) _distclean

################################################################################

_app: $(OBJS) | $(BIN_DIR)
	$(CXX) -o $(BIN_DIR)/$(BIN_NAME) $^ $(LDFLAGS)

_distclean: | _clean
	-rm -f $(BIN_DIR)/$(BIN_NAME)

_clean:
	-rm -rf $(OBJ_DIR)

$(OBJ_DIR)/%.opp : $(SRC)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(OBJS): | $(OBJ_SUBDIRS) $(OBJ_DIR)

$(OBJ_SUBDIRS):
	mkdir -p $(OBJ_DIR)/$@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

