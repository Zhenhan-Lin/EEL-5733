# Env related settings
BUILD	?= build
# Compiler Parameters
CC		?= gcc
CFLAGS	?= -Wall
# Object list
SRCS     = $(wildcard *.c)
INCS     = $(wildcard inc/*.h)
LIB_INCS = $(wildcard lib/*.h)
LIB_SRCS = $(wildcard lib/*.c)
# Default target
all: $(patsubst %.c,$(BUILD)/%,$(SRCS))
# Set up C suffixes & relationship between .cpp and .o files
$(BUILD)/%.o: %.c $(INCS) $(LIB_INCS)
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c -I./inc -I./lib -o $@ $<

$(BUILD)/lib/%.o: lib $(LIB_INCS)
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c -I./lib -o $@ $<

$(BUILD)/%: $(BUILD)/%.o $(patsubst %.c,$(BUILD)/%.o,$(LIB_SRCS))
	${CC} -o $@ $^;
	@chmod +x $@

BUFFER?=10
run: all
	$(BUILD)/location_updater $(BUFFER) < 'input.txt'

clean:
	rm -rf $(BUILD)

.PHONY: all clean

.PRECIOUS: $(BUILD)/lib/%.o $(BUILD)/%.o
