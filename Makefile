OBJS = string.o commsdc.o libsdc.o sdccmdr.o hirestxt/screen.o

all: libhirestxt.a UPDATE

hirestxt/screen.o:
	make -C hirestxt screen.o

libhirestxt.a:
	make -C hirestxt/hirestxt-0.3.1

UPDATE: sdccmdr.bin sdccmdr.dsk
	decb copy -r -2 -b $< sdccmdr.dsk,SDCCMDR.BIN

sdccmdr.dsk:
	decb dskini sdccmdr.dsk

sdccmdr.bin: $(OBJS) libhirestxt.a
	cmoc -osdccmdr.bin $(OBJS) -Lhirestxt/hirestxt-0.3.1 -lhirestxt-51x24 -lhirestxt

%.o: %.c
	cmoc -c $<

clean:
	make -C hirestxt clean
	make -C hirestxt/hirestxt-0.3.1 clean
	rm -f *.o *.dsk
