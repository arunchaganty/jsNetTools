# jsNetTools Makefilefile
# Author: Arun Chaganty <arunchaganty@gmail.com>
#

CC=g++
CFLAGS=-Wall -DXP_UNIX=1 -DMOZ_X11=1 -fPIC -g -Iinclude `pkg-config --cflags libxul`
CXXFLAGS=$(CFLAGS)
LDFLAGS=`pkg-config --libs libxul mozilla-plugin`
TARGETS=lib/libjsNetTools.so

INSTALLDIR=$(HOME)/.mozilla/plugins
IDLTOOL=$(HOME)/Projects/mozilla-central/ff-opt/dist/bin/xpidl
IDL_INCLUDEDIR=$(HOME)/Projects/mozilla-central/ff-opt/dist/idl/

all: $(TARGETS)

lib/libjsNetTools.so: obj/jsNetTools.o obj/jsInterface.o obj/NPN.o
	$(CC) $(CFLAGS) -shared $^ -o $@

obj/jsNetTools.o: src/jsNetTools.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

obj/jsInterface.o: src/jsInterface.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

obj/NPN.o: src/NPN.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

install: lib/libjsNetTools.so
	install $^ $(INSTALLDIR)

.PHONY: clean

clean:
	rm -rf bin/*
	rm -rf lib/*
	rm -rf obj/*

