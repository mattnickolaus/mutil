CC = gcc
CFLAGS = -Wall -g

SRC = mutil.c

# tagets 
.PHONY = all linux clean

all: linux

linux: mutil

mutil: $(SRC)
	$(CC) -o $@ $^ $(CFLAGS)
# !FYI - $@ is shorthand for the target and $^ similarly is for the prerequisites (like .c files)

