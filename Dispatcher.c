#include <string.h>

#include "Q280/Q280.c"
#include "Q240/Q240.c"

void printScreen() {
	char result[SCREEN_HEIGHT * (SCREEN_WIDTH * 2)];
	GPU_getScreen(result);
	result[sizeof(result) - 1] = '\0';
	puts(result);
}

int main(int argc, char **argv) {
    u16 version = 0;

    for(int i = 1; i < argc; i++) {
        if(argv[i][0] != '-') {
            char *file_ext = strchr(argv[i], ".");
            if(file_ext++ && atoi(file_ext)) {
                version = atoi(file_ext);
				
				if(version == 240) {
					readBin240(argv[i]);
					unpack240();
				} else if(version == 280) {
                    init280();
					readBin280(argv[i]);
				} else {
					puts("Architecture not supported");
				}

            } else {
                puts("Unknown filetype");
            }
			
		} else {
            switch(argv[i][1]) {
				case 'a':
					puts("Assembler not complete");
					return 0;
				case 's':
                    if(version == 240) {
                        speedTest240(argv[i][2]);
                    } else if(version == 280) {
                        speedTest280(argv[i][2]);
                    } else {
                        puts("Architecture not supported");
                    }

					return 0;
				case 'v':
					puts("BranchPU VM v0.7, Copyright (C) 2026 QuantumBranching");
					return 0;
				default:
					printf("Unknown flag: %s\n", argv[i]);
			}
        }
    }

    if(version == 240) {
        for(;;) {
            exec240(240);
            clear_screen();
            printState240();
            printScreen();
            puts("");
            sleepms(5);
        }
    } else if(version == 280) {
        while(TRUE) {
			exec280(240);
			clear_screen();
			printScreen();
			sleepms(5);
		}
    } else {
        puts("Architecture not supported");
    }
}