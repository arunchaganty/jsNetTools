# jsNetTools Makefilefile
# Author: Arun Chaganty <arunchaganty@gmail.com>
#

CC=gcc
CFLAGS=-Wall -DXP_UNIX=1 -DMOZ_X11=1 -fPIC -g `pkg-config --cflags libxul`
LDFLAGS=`pkg-config --libs libxul`
TARGETS=lib/jsNetTools.so

INSTALLDIR=$(HOME)/.mozilla/plugins

all: $(TARGETS)

lib/jsNetTools.so: obj/jsNetTools.o
	$(CC) $(CFLAGS) -shared $^ -o $@

obj/jsNetTools.o: src/jsNetTools.c
	$(CC) $(CFLAGS) -c $^ -o $@

install: lib/jsNetTools.so
	install $^ $(INSTALLDIR)



.PHONY: clean

clean:
	rm -rf bin/*
	rm -rf lib/*
	rm -rf obj/*

