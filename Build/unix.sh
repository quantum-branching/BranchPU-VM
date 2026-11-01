#!/usr/bin/env sh

# Assembler Object Files (Assumes you have lua 5.4)
cd Assembler
objcopy -I binary -O elf64-x86-64 Assembler.lua script.o
gcc script.o Assembler.c -o ../Build/Assembler.x86_64 -I/usr/include/lua5.4 -llua5.4
rm *.o
cd ..

# Virtual Machine Object Files
gcc Dispatcher.c -Ofast -flto -c -o Dispatcher.o
gcc Q240/Q240.c -O3 -c -o Q240.o
gcc Q242/Q242.c -O3 -c -o Q242.o
gcc Q280/Q280.c -Ofast -flto -c -o Q280.o
gcc IO/GPU.c -O3 -c -o GPU.o

# Link files together
gcc Dispatcher.o GPU.o Q240.o Q242.o Q280.o -flto -o Build/VM.x86_64
strip Build/VM.x86_64
rm *.o

