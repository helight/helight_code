CC=gcc
CFLAG=-lpthread -lrt

objbin=xserver simple
all: $(objbin)

xserver: xserver.c
	$(CC) $(CFLAG) $< -o $@
simple: simple.c
	$(CC) $(CFLAG) $< -o $@
	
clean:
	rm $(objbin)
