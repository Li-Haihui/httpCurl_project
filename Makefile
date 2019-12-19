# target
TARGET=httpDowntest.out

#path
BASEDIR := .
CODE_PATH := .
SUBDIRS := $(shell find $(CODE_PATH) -type d)
OUTPUT_DIR = $(CODE_PATH)/bin

#libpath
LIB_FILE += -lcurl -lpthread
LIB_PATH += -L$(BASEDIR)/thirdparty/curl

#include 
INCLUDE_PATH += -I$(BASEDIR)/include/thread_pool
INCLUDE_PATH += -I$(BASEDIR)/include/curl
INCLUDE_PATH += -I$(BASEDIR)/include/curl_down
INCLUDE_PATH += -I$(BASEDIR)/include/http_down

#src
SRC += $(shell find $(CODE_PATH)/src/thread_pool -name '*.cpp')
SRC += $(shell find $(CODE_PATH)/src/curl_down -name '*.cpp')
SRC += $(shell find $(CODE_PATH)/src/http_down -name '*.cpp')
SRC += $(shell find $(CODE_PATH)/src -name '*.cpp')

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

all:dir httpDowntest

dir:
	mkdir -p $(OUTPUT_DIR);
	@for val in $(SUBDIRS);do\
		mkdir -p $(OUTPUT_DIR)/$${val};\
	done;
	
httpDowntest:$(OUTPUT_DIR)/$(TARGET)

$(OUTPUT_DIR)/$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB_PATH) $(LIB_FILE)

$(OUTPUT_DIR)/%.o:%.cpp
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -c $< -o $@
	
clean:
	$(RM) $(RM_FLAGS) $(OUTPUT_DIR)

.PHONY:all dir httpDowntest copy clean	
	

