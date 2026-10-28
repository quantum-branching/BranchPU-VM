@echo off

:: Check if a universal CC variable is already defined by the user/system
if "%CC%"=="" (
    echo CC is not set. Searching for available compilers...
    
    :: Check if GCC is available in the system PATH
    where gcc >nul 2>nul
    if %errorlevel% equ 0 (
        set CC=gcc
    ) else (
        :: Check if MSVC (cl.exe) is available
        where cl >nul 2>nul
        if %errorlevel% equ 0 (
            set CC=cl
        )
    )
)

:: Verify if a compiler was found
if "%CC%"=="" (
    echo Error: No C compiler found in PATH or %%CC%% variable.
    exit /b 1
)

:: Compilation
%CC% Dispatcher.c -Ofast -flto -c -o Dispatcher.asm
%CC% Q240/Q240.c -O3 -c -o Q240.asm
%CC% Q242/Q242.c -O3 -c -o Q242.asm
%CC% Q280/Q280.c -Ofast -flto -c -o Q280.asm
%CC% IO/GPU.c -O3 -c -o GPU.asm
%CC% Dispatcher.asm GPU.asm Q240.asm Q242.asm Q280.asm -flto -o Build/VM.exe
del *.asm