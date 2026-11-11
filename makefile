CC = gcc

LUAFLAGS = $(shell pkg-config --cflags luajit)
LDFLAGS = $(shell pkg-config --libs luajit)

my_program: Dispatcher.o GPU.o Q240.o Q242.o Q280.o Assembler.o script.o
	$(CC) $^ $(LDFLAGS) -flto -o VM.x86_64
	strip VM.x86_64

Dispatcher.o: Dispatcher.c
	$(CC) $^ -Ofast -flto -c -o $@

GPU.o: IO/GPU.c
	$(CC) $^ -O3 -c -o $@

Q240.o: Q240/Q240.c
	$(CC) $^ -O3 -c -o $@

Q242.o: Q242/Q242.c
	$(CC) $^ -O3 -c -o $@

Q280.o: Q280/Q280.c
	$(CC) $^ -O3 -c -o $@

Assembler.o: Assembler/Assembler.c
	$(CC) $^ -O3 -flto -c $(LUAFLAGS) -o $@

script.o: Assembler/Assembler.lua
	cd Assembler && \
	objcopy -I binary -O elf64-x86-64 Assembler.lua ../$@ && \
	cd ..

.PHONY: clean
clean:
	rm -f *.o
