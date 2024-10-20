TARGET := gerex

# preference order clang > gcc
ifeq ($(shell command -v clang),)
	CC := gcc
else
	CC := clang
endif

C_FLAGS := -Wall -Wextra -Wpedantic -Werror -std=c2x
C_DEBUG_FLAGS := -g -pg
C_RELEASE_FLAGS := -O3

# preference order mold > lld > ld
ifeq ($(shell command -v mold),)
	ifeq  ($(shell command -v lld),)
		C_LINK := -fuse-ld=ld
	else
		C_LINK := -fuse-ld=lld
	endif
else
	C_LINK := -fuse-ld=mold
endif

.PHONY: all clean debug release

all: debug

debug: C_FLAGS += $(C_DEBUG_FLAGS)
debug: $(TARGET)

release: C_FLAGS += $(C_RELEASE_FLAGS)
release: $(TARGET)

$(shell mkdir -p build)
$(shell mkdir -p build/objects)

SOURCE_FILES := $(wildcard src/*.c)
OBJECT_FILES := $(patsubst src/%.c, build/objects/%.o, $(SOURCE_FILES))

$(TARGET): $(OBJECT_FILES)
	$(CC) $(C_FLAGS) $(C_LINK) $^ -o build/$@

build/objects/%.o: src/%.c
	$(CC) $(C_FLAGS) -c $^ -o $@

clean:
	rm -rf build
