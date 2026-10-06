gcc Dispatcher.c -Ofast -flto -c -o Dispatcher.o
gcc Q240/Q240.c -O3 -c -o Q240.o
gcc Q280/Q280.c -Ofast -flto -c -o Q280.o
gcc IO/GPU.c -O3 -c -o GPU.o
gcc Dispatcher.o GPU.o Q240.o Q280.o -flto -o Build/VM.x86_64
del *.o