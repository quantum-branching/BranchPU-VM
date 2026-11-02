#!/usr/bin/env sh

# Assembler Object Files (Assumes you have lua 5.4)
cd Assembler
objcopy -I binary -O elf64-x86-64 Assembler.lua ../script.o
gcc Assembler.c -O3 -flto -c -I/usr/include/lua5.4 -o ../Assembler.o 
cd ..

# Virtual Machine Object Files
gcc Dispatcher.c -Ofast -flto -c -o Dispatcher.o
gcc Q240/Q240.c -O3 -c -o Q240.o
gcc Q242/Q242.c -O3 -c -o Q242.o
gcc Q280/Q280.c -O3 -c -o Q280.o
gcc IO/GPU.c -O3 -c -o GPU.o

# Link files together
gcc Dispatcher.o GPU.o Q240.o Q242.o Q280.o Assembler.o script.o -llua5.4 -flto -o Build/VM.x86_64
strip Build/VM.x86_64
rm *.o

