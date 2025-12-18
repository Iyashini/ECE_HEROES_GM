CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -I.
LDFLAGS=

SRCS=main.c display.c game_systeme.c
OBJS=$(SRCS:.c=.o)
TARGET=match3

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
