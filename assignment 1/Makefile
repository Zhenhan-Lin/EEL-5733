TARGETS := calendar_filter.bin email_filter.bin location_updater.bin
# Compiler Parameters
CC      = gcc
CFLAGS  = -Wall

all: $(TARGETS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.bin: %.o
	${CC} -o $@ $(@:.bin=.o);

