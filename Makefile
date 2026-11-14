# --- Variables ---
CC     = gcc
CFLAGS = -Wall -Wextra -g
LDLIBS = -lraylib -lm

SRC = $(wildcard src/*.c src/*/*.c)
OBJ = $(SRC:src/%.c=build/%.o)

# --- Executable (first rule = what `make` builds) ---
chip8: $(OBJ)
	$(CC) $^ -o $@ $(LDLIBS)

# --- Compile each .c into a .o ---
build/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# --- Cleanup ---
.PHONY: clean
clean:
	rm -rf build
	rm -f chip8