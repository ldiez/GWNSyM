# Makefile wrapper for waf

all:
	./waf configure 
	./waf build_debug

configure:
	./waf configure

build:
	./waf build_debug

install:

clean:
	./waf distclean

distclean:
	./waf distclean
	