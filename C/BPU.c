#include <stdio.h>
#include <stdint.h>

uint8_t instructions[2048];
uint8_t registers[256];

uint8_t instruction;
uint8_t accumulator;

void BPUcompile(char* filename) {
	//File being read
	FILE *fptr;
	//Line being read
	char line[128];
	
	//Opens the requested file
	fptr = fopen(filename, "r");
	
	//Reads requested file
	while(fgets(line, 128, fptr)) {
		//Replace with compilation logic
		printf("%s", line);
	}
	
	// Close the file
	fclose(fptr);
}

int main(int argc, char* argv[]) {
	BPUcompile(argv[1]);
	return 0;
}
