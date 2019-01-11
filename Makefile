CC = gcc
C_FLAGS = -Isrc -Lsrc -Wall
LDFLAGS = -Wall

OUT_DIR = test/

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)testbin.out: src/CANlib.o src/drivers/src/test.o test/main.o
	$(CC) $^ $(LDFLAGS) -o $@
