# By Pierre Sarrazin <http://sarrazip.com/>
# This file is in the public domain.

PACKAGE = hirestxt
VERSION = 0.3.1

LIBSRC = $(PACKAGE).c
LIB42SRC = writeCharAt_42cols.c font5x8.c
LIB51SRC = writeCharAt_51cols.c font4x8.c
LIBHEADERS = $(PACKAGE).h font4x8.h font5x8.h
STATICLIB = lib$(PACKAGE).a
STATICLIB42 = lib$(PACKAGE)-42x24.a
STATICLIB51 = lib$(PACKAGE)-51x24.a

DEMOSRC = $(PACKAGE)-demo.c
DEMOBIN = $(PACKAGE).bin

DISTFILES = README Makefile $(LIBHEADERS) $(LIBSRC) $(LIB42SRC) $(LIB51SRC) $(DEMOSRC)

TARGET = coco
ORG = #--org=2800
INTERMEDIATE = #--intermediate
NDEBUG = -DNDEBUG
OPTIM =
CFLAGS = --$(TARGET) $(NDEBUG) $(INTERMEDIATE) $(OPTIM)
LDFLAGS = --$(TARGET) $(ORG) $(INTERMEDIATE)

DEMOOBJ = $(DEMOSRC:.c=.o)
LIBOBJ = $(LIBSRC:.c=.o)
LIB42OBJ = $(LIB42SRC:.c=.o)
LIB51OBJ = $(LIB51SRC:.c=.o)

CMOC = cmoc
LWAR = lwar
TAR = tar

all: $(DEMOBIN)

$(DEMOBIN): $(DEMOOBJ) $(STATICLIB) $(STATICLIB42) $(STATICLIB51)
	$(CMOC) -o $(DEMOBIN) $(LDFLAGS) $(DEMOOBJ) -L. -l$(PACKAGE) -l$(PACKAGE)-42x24 -l$(PACKAGE)-51x24

$(STATICLIB): $(LIBOBJ)
	$(LWAR) --create $(STATICLIB) $(LIBOBJ)
$(STATICLIB42): $(LIB42OBJ)
	$(LWAR) --create $(STATICLIB42) $(LIB42OBJ)
$(STATICLIB51): $(LIB51OBJ)
	$(LWAR) --create $(STATICLIB51) $(LIB51OBJ)

%.o: %.c
	$(CMOC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(DEMOBIN) $(DEMOOBJ) $(STATICLIB) $(STATICLIB42) $(STATICLIB51) $(LIBOBJ) $(LIB42OBJ) $(LIB51OBJ)

.PHONY: dist
dist:
	$(TAR) -czf $(PACKAGE)-$(VERSION).tar.gz --transform 's,^,$(PACKAGE)-$(VERSION)/,' $(DISTFILES)

# Automatic prerequisite generation.
%.d: %.c
	$(CMOC) --deps-only $(CFLAGS) $<

.PHONY: cleandeps
cleandeps:
	rm -f $(DEMOSRC:.c=.d) $(LIBSRC:.c=.d) $(LIB42SRC:.c=.d) $(LIB51SRC:.c=.d)

include $(DEMOSRC:.c=.d)
include $(LIBSRC:.c=.d)
include $(LIB42SRC:.c=.d)
include $(LIB51SRC:.c=.d)
