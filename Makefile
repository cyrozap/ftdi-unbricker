CC = gcc

TARGETS = brick unbrick

CFLAGS += -Wall $(shell pkg-config --cflags libftdi1)
LDFLAGS += $(shell pkg-config --libs libftdi1)

all: $(TARGETS)

brick: brick.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o brick brick.c

unbrick: unbrick.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o unbrick unbrick.c

clean:
	rm -f $(TARGETS)

.PHONY: all clean
