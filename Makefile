# target
TARGET=threadpooltest.out

#path
BASEDIR := .
CODE_PATH := .
SUBDIRS := $(shell find $(CODE_PATH) -type d)
OUTPUT_DIR = $(CODE_PATH)/bin

#libpath
LIB_FILE += -lpthread
#LIB_PATH += -L$(BASEDIR)/

#include 
INCLUDE_PATH += -I$(BASEDIR)/include/thread_pool

#src
SRC += $(shell find $(CODE_PATH)/src/thread_pool -name '*.cpp')

#object
OBJS := $(SRC:%.cpp=$(OUTPUT_DIR)/%.o)

#g++ tool
CC = g++
RM = rm
RM_FLAGS = -rf

#compile flag
DEBUG = true
PLATFORM = false
CFLAGS := -fpic -Wall -fkeep-inline-functions
PLAT_CFLAGS := $(CFLAGS)

ifeq ($(DEBUG),true)
	CFLAGS += -g
else
	CFLAGS += -g -O2
endif

all:dir threadpooltest

dir:
	mkdir -p $(OUTPUT_DIR);
	@for val in $(SUBDIRS);do\
		mkdir -p $(OUTPUT_DIR)/$${val};\
	done;
	
threadpooltest:$(OUTPUT_DIR)/$(TARGET)

$(OUTPUT_DIR)/$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB_FILE)

$(OUTPUT_DIR)/%.o:%.cpp
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -c $< -o $@
	
clean:
	$(RM) $(RM_FLAGS) $(OUTPUT_DIR)

.PHONY:all dir threadpooltest copy clean	
	

