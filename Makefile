# --- Variables ---
CC     = gcc
CFLAGS = -Wall -Wextra -g
LDLIBS = -lraylib -lm
RAYLIB_WEB = ~/Code/libs/raylib/src

SRC = $(wildcard src/*.c src/*/*.c)
OBJ = $(SRC:src/%.c=build/%.o)

# --- Executable (first rule = what `make` builds) ---
chip8: $(OBJ)
	$(CC) $^ -o $@ $(LDLIBS)

# --- Compile each .c into a .o ---
build/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# --- Compile web version ---
web: 
	mkdir -p docs
	emcc $(SRC) -o docs/index.html -I $(RAYLIB_WEB) $(RAYLIB_WEB)/libraylib.web.a -sUSE_GLFW=3 -sEXPORTED_RUNTIME_METHODS=ccall,FS -DPLATFORM_WEB -Os --preload-file roms --shell-file web_shell.html
# --- Cleanup ---
.PHONY: clean web
clean:
	rm -rf build
	rm -f chip8