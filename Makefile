CC      := gcc
CFLAGS  := -std=c17 -Wall -Wextra -Werror -D_POSIX_C_SOURCE=200809L -Iinclude -O2 -g
LDFLAGS :=

TARGET  := text-editor
SRCS    := $(wildcard src/*.c)
OBJS    := $(SRCS:src/%.c=build/%.o)
DEPS    := $(OBJS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -rf build $(TARGET)
