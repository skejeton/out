compile_command=clang -fsanitize=address -g -I/usr/include/SDL2 -lSDL2main -lSDL2 -lc -lm ./src/*.c -o ./bin/linux_version -Wuninitialized -Wmaybe-uninitialized -Wall -Wextra -Wcast-align -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wdouble-promotion -Wshadow -Wformat=2 -Werror=implicit-function-declaration -Werror=incompatible-pointer-types -Werror=discarded-qualifiers -Werror=int-to-pointer-cast -Werror=int-conversion -Werror=return-type -Werror=maybe-uninitialized -Werror=uninitialized -Werror=double-promotion

compile_command_wasm=emcc -O2 -s ASYNCIFY=1 -s ALLOW_MEMORY_GROWTH=1 -s USE_SDL=2 ./src/*.c --preload-file ASSETS -o ./bin/version_wasm.html --shell-file ./shell.html

preamble:
	if [ ! -d "./bin" ]; then mkdir bin; fi

linux: preamble
	$(compile_command)

windows: preamble
	x86_64-w64-mingw32-gcc -c ./src/*.c
	x86_64-w64-mingw32-gcc -o ./bin/version_windows.exe *.o -lSDL2main -lSDL2
	rm *.o

wasm: preamble
	$(compile_command_wasm)

# So far it only runs linux version
run: linux preamble
	./bin/linux_version

