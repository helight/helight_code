CFLAGS ?= -g -O2 -Wall
LDFLAGS ?=
#CPPFLAGS += -DPRINTDEBUG

VER_MAJOR = 0
VER_MINOR = 3
VER_PATCH = 0
VER=$(VER_MAJOR).$(VER_MINOR).$(VER_PATCH)

INSTALL ?= /usr/bin/install -c
MKDIR ?= /bin/mkdir -p
CP ?= /bin/cp -fd
LN ?= /bin/ln -fs

INSTALLDIR ?= /usr/local
LIBDIR = $(INSTALLDIR)/lib
INCLUDEDIR = $(INSTALLDIR)/include

# targets to build with 'make all'
TARGETS = credis-test2 

all: $(TARGETS)

credis-test2: credis-test2.o credis.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(CPPFLAGS) -o $@ $^

credis.o: credis.c credis.h Makefile
	$(CC) -c -fPIC $(CFLAGS) $(CPPFLAGS) -o $@ credis.c

clean:
	rm -f *.o *~ *.so* $(TARGETS)
