DECB = 1

OBJS = string.o commsdc.o libsdc.o sdccmdr.o

CC=cmoc
CFLAGS= -DDECB=1
DEPS=libhirestxt.a
OBJS+= hirestxt/screen.o
LIBSINC=-Lhirestxt/hirestxt-0.3.1
LIBS=-lhirestxt-51x24 -lhirestxt
DISK=UPDATE

all: $(DEPS) $(DISK)

os9/screen.o: os9/screen.c
	$(CC) $(CFLAGS) $(INCS) -o$@ -c $<

hirestxt/screen.o:
	make -C hirestxt screen.o

libhirestxt.a:
	make -C hirestxt/hirestxt-0.3.1

UPDATE: sdccmdr.bin sdccmdr-decb.dsk
	decb copy -r -2 -b $< sdccmdr-decb.dsk,SDCCMDR.BIN

sdccmdr-decb.dsk:
	decb dskini sdccmdr.dsk

sdccmdr.bin: $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) -osdccmdr.bin $(OBJS) $(LIBSINC) $(LIBS)

sdccmdr-os9.DSK:
	os9 format sdccmdr-os9.dsk
	os9 makdir sdccmdr-os9.dsk,os9

update-os9:
	os9 copy -l -r *.c *.h sdccmdr-os9.DSK,
	os9 copy -l -r Makefile.os9 sdccmdr-os9.DSK,Makefile
	os9 copy -l -r os9/*.c os9/*.h sdccmdr-os9.DSK,os9

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -c $<

clean:
	make -C hirestxt clean
	make -C hirestxt/hirestxt-0.3.1 clean
	rm -f *.o *.bin *.BIN

