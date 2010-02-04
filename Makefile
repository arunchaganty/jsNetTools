# jsNetTools Makefilefile
# Author: Arun Chaganty <arunchaganty@gmail.com>
#

CC=g++
CFLAGS=-Wall -DXP_UNIX=1 -DMOZ_X11=1 -fPIC -g -Iinclude `pkg-config --cflags libxul`
CXXFLAGS=$(CFLAGS)
LDFLAGS=`pkg-config --libs libxul` -lpthread
TARGETS=lib/libjsNetTools.so

INSTALLDIR=$(HOME)/.mozilla/plugins
VERSION=0.2

SRCFILES=include/ doc/ misc/ src/ tests/ Makefile Doxyfile README 
DISTFILES=lib/libjsNetTools.so misc/install.sh doc/ tests/ README

all: $(TARGETS)

lib/libjsNetTools.so: obj/jsNetTools.o obj/jsInterface.o obj/NPN.o obj/pingPlugin.o
	if [ ! -e lib ]; then mkdir lib; fi;
	$(CC) $(CFLAGS) -shared $^ -o $@

obj/jsNetTools.o: src/jsNetTools.cpp
	if [ ! -e obj ]; then mkdir obj; fi;
	$(CC) $(CFLAGS) -c $^ -o $@

obj/jsInterface.o: src/jsInterface.cpp
	if [ ! -e obj ]; then mkdir obj; fi;
	$(CC) $(CFLAGS) -c $^ -o $@

obj/NPN.o: src/NPN.cpp
	if [ ! -e obj ]; then mkdir obj; fi;
	$(CC) $(CFLAGS) -c $^ -o $@

obj/pingPlugin.o: src/pingPlugin.cpp
	if [ ! -e obj ]; then mkdir obj; fi;
	$(CC) $(CFLAGS) -c $^ -o $@

install: lib/libjsNetTools.so
	install $^ $(INSTALLDIR)

src-dist: 
	rm -rf jsNetTools-src-$(VERSION)
	mkdir jsNetTools-src-$(VERSION)
	cp -rf $(SRCFILES) jsNetTools-src-$(VERSION)/
	tar -czf jsNetTools-src-$(VERSION).tar.gz jsNetTools-src-$(VERSION)/
	rm -rf jsNetTools-src-$(VERSION)

bin-dist: all
	rm -rf jsNetTools-$(VERSION)
	mkdir jsNetTools-$(VERSION)
	cp -rf $(DISTFILES) jsNetTools-$(VERSION)/
	tar -czf jsNetTools-$(VERSION).tar.gz jsNetTools-$(VERSION)/
	rm -r jsNetTools-$(VERSION)

.PHONY: clean doc

doc: 
	doxygen

clean:
	rm -rf bin/*
	rm -rf lib/*
	rm -rf obj/*

