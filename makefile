LDIR=./lib
CC=gcc
CFLAGS= -Wall
LDFLAGS= -Llib -lcrypto -lciphers -linfodef -lrandombytes -lm
TARGET= poker
NEED= print_card

.PHONY: all clean $(TARGET)

default: $(TARGET) clean
all: default

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): %: %.o $(NEED).o
	$(CC) $< $(NEED).o $(LDFLAGS) -o $@

clean:
	-rm -f *.o
