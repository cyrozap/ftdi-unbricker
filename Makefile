CC ?= gcc

TARGETS = brick unbrick

CFLAGS += -Wall $(shell pkg-config --cflags libftdi1)
LDFLAGS += $(shell pkg-config --libs libftdi1)

all: $(TARGETS)

%: %.c
	$(CC) $< $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f $(TARGETS)

.PHONY: all clean
