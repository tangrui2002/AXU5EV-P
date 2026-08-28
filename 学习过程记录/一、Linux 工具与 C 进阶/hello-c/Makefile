CC := gcc
CFLAGS_DEBUG := -Wall -Wextra -g -O0 -Iinclude
CFLAGS_RELEASE := -Wall -Wextra -O2 -DNDEBUG -Iinclude

TARGET := hello
SRCS := src/main.c src/math_utils.c
OBJS := $(SRCS:.c=.o)

.PHONY: all clean debug release
all: debug 

debug: CFLAGS = $(CFLAGS_DEBUG)

release: CFLAGS = $(CFLAGS_RELEASE)

release: clean

debug release: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
