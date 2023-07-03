CC=m6809-unknown-gcc
CFLAGS=-Os -DFUZIX
IFLAGS=-I/home/n6il/src/FUZIX/Library/include -I/home/n6il/src/FUZIX/Library/include/6809

OBJS=screen.o

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $<

clean:
	rm -f $(OBJS)
