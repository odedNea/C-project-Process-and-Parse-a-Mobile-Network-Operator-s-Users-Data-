# Determine compiler
CC=gcc

# Complilation flags
CFLAGS=-ansi -pedantic -Wall -Werror -Wconversion -g -Wvariadic-macros -pthread -fPIC -m32 -D _GNU_SOURCE #  -Wno-variadic-macros

# Linking flags
LDFLAGS=

# My variables
TARGET=UI

OBJS2=../Shared/channel.o

$(TARGET): $(OBJS2)


clean:
	@echo "Deleting compilation files"
	rm -f $(TARGET) $(OBJS2)

#rebuild: clean $(TARGET1)


run: 
	./$(TARGET1)
#############

# sharedQue.o: sharedQue.h
