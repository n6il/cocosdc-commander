OBJS = string.o commsdc.o libsdc.o sdccmdr.o hirestxt/screen.o

all: UPDATE


UPDATE: sdccmdr.bin sdccmdr.dsk
	decb copy -r -2 -b $< sdccmdr.dsk,SDCCMDR.BIN

sdccmdr.dsk:
	decb dskini sdccmdr.dsk

sdccmdr.bin: $(OBJS)
	cmoc -osdccmdr.bin string.o commsdc.o libsdc.o sdccmdr.o hirestxt/screen.o -Lhirestxt/hirestxt-0.3.1 -lhirestxt-51x24 -lhirestxt

%.o: %.c
	cmoc -c $<

clean:
	rm -f *.o *.dsk
