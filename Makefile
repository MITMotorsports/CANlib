CC = gcc
C_FLAGS = -Isrc -Lsrc -Wall
LDFLAGS = -Wall

OUT_DIR = bin/

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

testbin: src/CANlib.o src/drivers/src/test.o test/main.o
	$(CC) $^ $(LDFLAGS) -o $@
