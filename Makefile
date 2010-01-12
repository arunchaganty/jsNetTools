# jsNetTools Makefilefile
# Author: Arun Chaganty <arunchaganty@gmail.com>
#

CC=gcc
CFLAGS=-Wall -DXP_UNIX=1 -DMOZ_X11=1 -fPIC -g -Iinclude `pkg-config --cflags libxul`
LDFLAGS=`pkg-config --libs libxul`
TARGETS=lib/jsNetTools.so

INSTALLDIR=$(HOME)/.mozilla/plugins
IDLTOOL=$(HOME)/Projects/mozilla-central/ff-opt/dist/bin/xpidl
IDL_INCLUDEDIR=$(HOME)/Projects/mozilla-central/ff-opt/dist/idl/

all: $(TARGETS)

lib/jsNetTools.so: obj/jsNetTools.o obj/jsInterface.o
	$(CC) $(CFLAGS) -shared $^ -o $@

obj/jsNetTools.o: src/jsNetTools.c
	$(CC) $(CFLAGS) -c $^ -o $@

obj/jsInterface.o: src/jsInterface.c
	$(CC) $(CFLAGS) -c $^ -o $@

include/jsINetTools.h lib/jsINetTools.xpt: src/jsINetTools.idl
	$(IDLTOOL) -w -I $(IDL_INCLUDEDIR) -m header $^ -e jsINetTools.h
	$(IDLTOOL) -w -I $(IDL_INCLUDEDIR) -m typelib $^ -e jsINetTools.xpt
	mv jsINetTools.h include/
	mv jsINetTools.xpt lib/

install: lib/jsNetTools.so
	install $^ $(INSTALLDIR)



.PHONY: clean

clean:
	rm -rf bin/*
	rm -rf lib/*
	rm -rf obj/*

