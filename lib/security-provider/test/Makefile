# NAME      = security_test

LIBS			= -lssl -lcrypto -lcurl
CFLAGS		= -I.. -Wall -Werror -ggdb

# object files
OBJECT_FILES = $(shell find ./ -type f -name '*.o')

# MAC OS temp files
MAC_TEMP_FILES = $(shell find ./ -type f -name '._*')

# # source files
# SOURCES = $(NAME).c

# # object files
# OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

all: security_test authen_test

authen_test: authen_test.o
	$(CC) $(CFLAGS) authen_test.o -o authen_test $(LIBS)

security_test: security_test.o
	$(CC) $(CFLAGS) security_test.o -o security_test $(LIBS)

clean:
	@- $(RM) authen_test
	@- $(RM) security_test
	@- $(RM) $(OBJECT_FILES)
	@- $(RM) $(MAC_TEMP_FILES)

test: security_test authen_test
	@- ./authen_test
	@- ./security_test

mem_check: security_test authen_test
	@- valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes ./authen_test
	@- valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes ./security_test
