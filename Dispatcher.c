#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Q280/Q280.h"
#include "Q240/Q240.h"
#include "Q242/Q242.h"
#include "Assembler/Assembler.h"

#include "IO/GPU.h"
#include "OS.c"

void printScreen() {
	char result[SCREEN_HEIGHT * (SCREEN_WIDTH * 2)];
	GPU_getScreen(result);
	result[sizeof(result) - 1] = '\0';
	puts(result);
}

int main(int argc, char **argv) {
	unsigned short version = 0;

	for(int i = 1; i < argc; i++) {
		if(argv[i][0] != '-') {
			char *file_ext = strchr(argv[i], '.');

			if(file_ext && strtol(file_ext + 2, NULL, 10)) {
				version = strtol(file_ext + 2, NULL, 10);

				switch(version) {
					case 240:
						readBin240(argv[i]);
						unpack240();
						break;
					case 242:
						init242();
						readBin242(argv[i]);
						break;
					case 280:
						init280();
						readBin280(argv[i]);
						break;
					default:
						printf("Architecture not supported: .q%dx\n", version);
				}

			} else {
				if(strcmp(file_ext, ".bpux")) {
					printf("Can't read file extension: %s\n", file_ext);
				} else {
					version = 280;
					init280();
					readBin280(argv[i]);
				}
			}
			
		} else {
			switch(argv[i][1]) {
				case 'a':
					assembler__q242(argc - i, argv + i);
					return 0;
				case 's':
					switch(version) {
						case 240:
							speedTest240(argv[i][2]);
							break;
						case 242:
							speedTest242(argv[i][2]);
							break;
						case 280:
							speedTest280(argv[i][2]);
							break;
						default:
							printf("Architecture not supported for testing speed: .q%dx\n", version);
							break;
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
	} else if(version == 242) {
		for(;;) {
			exec242(240);
			// clear_screen();
			// printScreen();
			// puts("");
			sleepms(5);
		}
	} else if(version == 280) {
		for(;;) {
			exec280(240);
			clear_screen();
			printState280();
			printScreen();
			sleepms(5);
		}
	} else {
		printf("Architecture not supported: .q%03dx\n", version);
	}
}
