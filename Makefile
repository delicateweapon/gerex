$(shell mkdir -p build && mkdir -p build/objects && mkdir -p test/)

TARGET := build/capella

CC := clang
C_FLAGS := -Wall -Wextra -Werror -Wpedantic -std=c2x
C_LINK_FLAGS := -fuse-ld=mold

C_DEBUG_FLAGS := -g -pg
C_RELEASE_FLAGS := -03

.PHONY: all clean release debug debugrun test

all: debug

debug: C_FLAGS += $(C_DEBUG_FLAGS)
debug: $(TARGET)

debugrun: debug
	lldb -- build/capella

release: C_FLAGS += $(C_RELEASE_FLAGS)
release: $(TARGET)

SOURCE_FILES := $(wildcard src/*.c)
OBJECT_FILES := $(patsubst src/%.c, build/objects/%.o, $(SOURCE_FILES))

$(TARGET): $(OBJECT_FILES)
	$(CC) $(C_FLAGS) $(C_LINK_FLAGS) $^ -o $@

build/objects/%.o: src/%.c
	$(CC) $(C_FLAGS) $^ -c -o $@

clean:
	rm -rf build
