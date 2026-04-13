#include <stdio.h>
#include <stdlib.h>

#define NUM_REGISTERS 256

struct Stack {
	int data[NUM_REGISTERS];
	char mod[NUM_REGISTERS];
	int current;
};

void push(struct Stack stack, char mod, int value) {
	if (stack.current < NUM_REGISTERS) {
		stack.data[stack.current] = value;
		stack.mod[stack.current] = mod;
		stack.current++;
	} else {
		fprintf(stderr, "Stack overflow\n");
	}
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

	char line[256];
	char *current = &line[0];
	int integer = 0;
	struct Stack stack;
	

    while(fgets(line, sizeof(line), file)) {
		while(*current != '\0') {
			//If space, push integer to stack
			if(*current == ' ') {
				push(stack, '#', integer); //Have not implemented registers yet
				integer = 0;
			}
			current++;
		}

		// Process the line
		printf("%s", line);
	}

    fclose(file);
    return 0;
}
