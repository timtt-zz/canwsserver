DESTDIR =
PREFIX = /usr/local

INCLUDES = -Ikernel/3.0/include 

MAKEFLAGS = -k

CFLAGS    = -O2 -Wall -Wno-parentheses $(INCLUDES) \
	    -fno-strict-aliasing \
	    -DSO_RXQ_OVFL=40 \
	    -DPF_CAN=29 \
	    -DAF_CAN=PF_CAN

PROGRAMS = canwsserver

all: $(PROGRAMS)
	
clean:
	rm -f $(PROGRAMS) *.o

install:
	mkdir -p ~/bin
	cp -f $(PROGRAMS) $(DESTDIR)$(PREFIX)/bin

distclean:
	rm -f $(PROGRAMS) *.o *~

canwsserver.o:	helpers.h

canwsserver:	canwsserver.o	helpers.o	wsserver.o	lib/websockets/*.c