# jsNetTools Makefilefile
# Author: Arun Chaganty <arunchaganty@gmail.com>
#

CC=gcc
CFLAGS=-Wall -DXP_UNIX=1 -DMOZ_X11=1 -fPIC -g
LDFLAGS=
TARGETS=lib/jsNetTools.so

all: $(TARGETS)

lib/jsNetTools.so: obj/jsNetTools.o
	$(CC) $(CFLAGS) -shared $^ -o $@

obj/jsNetTools.o: src/jsNetTools.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean

clean:
	rm -rf bin/*
	rm -rf lib/*
	rm -rf obj/*

