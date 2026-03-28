#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

uint8_t instructions[2048];
uint8_t arguments[2048];										
uint8_t registers[256];

int instruction;
uint8_t accumulator;

char* getSlice(char text[], char delimiter, int idx) {
    int start = 0;
    int currentIdx = 0;
    int i = 0;
    // Find the start of the desired slice
    while (text[i] != '\0') {
        if (currentIdx == idx) {
            start = i;
            break;
        }
        if (text[i] == delimiter) {
            currentIdx++;
        }
        i++;
    }
    // If index not found
    if (currentIdx != idx) {
        return NULL;
    }
    // Find the end of the slice
    int end = start;
    while (text[end] != '\0' && text[end] != delimiter) {
        end++;
    }
    // Allocate memory for the slice (+1 for null terminator)
    int length = end - start;
    char *slice = (char *)malloc(length + 1);
    if (!slice) {
		return NULL;
	}
    strncpy(slice, text + start, length);
    slice[length] = '\0';
    return slice;
}

bool startsWith(char text[], char sub[]) {
	int size = strlen(sub);
	int idx = 0;
	while(text[idx] == sub[idx]) {
		idx++;
		if(idx == size) {
			return true;
		}
	}
	return false;
}

void BPUcompile(char* filename) {
	//Instructions Set
	char isa[20][4] = {"JMP","ADD","SUB","LSH","RSH","AND","OR","XOR","LDA","STA","CND","PSH","POP","CMP","ICP","STK","RPA","RPR","WPA","WPR"};
	//File being read
	FILE *fptr;
	//Line being read
	char line[128];
	
	//Set Global Variables
	instruction = 0;
	
	//Opens the requested file
	fptr = fopen(filename, "r");
	
	//Reads requested file
	while(fgets(line, 128, fptr)) {
		int isaSize = sizeof(isa);
		for(int i = 0; i < isaSize; i++) {
			if(startsWith(line,isa[i])) {
				instructions[instruction] = i;
				instruction++;
			}
		}
	}
	
	// Close the file
	fclose(fptr);
}

int main(int argc, char* argv[]) {
	BPUcompile(argv[1]);
	return 0;
}
