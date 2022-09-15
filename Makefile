# app name
PROGRAM = noc-client

# program source files
SOURCE_FILES = $(shell find src -type f -name '*.c')

# object files
OBJECT_FILES = $(shell find ./ -type f -name '*.o')

# MAC OS temp files
MAC_TEMP_FILES = $(shell find ./ -type f -name '._*')

# collect all .cc source files
SOURCES = $(SOURCE_FILES)

# object files
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

# link flags
LDFLAGS = -lmosquitto -lpthread -lcurl -luuid -lssl -lcrypto -lcjson

# target flags
CXXFLAGS = -O2 -std=c11 -Wall -Wextra -fcommon #-ggdb

# include -I
CFLAGS = -Isrc -Ilib

# defines
CFLAGS += -DEZM_VERSION=\"${EZM_VERSION}\"
CFLAGS += -DLOG_USE_COLOR -DLOG_USE_FILENAME -DLOG_USE_STDERR

.PHONY: noc-client clean

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) $(CFLAGS) $(CXXFLAGS) $(OBJECTS) -o $(PROGRAM) $(LDFLAGS)

clean:
	@- $(RM) $(PROGRAM)
	@- $(RM) $(OBJECT_FILES)
	@- $(RM) $(MAC_TEMP_FILES)
