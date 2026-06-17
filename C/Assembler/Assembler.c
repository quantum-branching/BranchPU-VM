#include <stdio.h>
#include "Token.c"

#define ASSEMBLY_SIZE 32768

char assembly[ASSEMBLY_SIZE];

void readAssembly(const char *filename) {
	FILE *file = fopen(filename, "rb");
	if (file) {
		fread(assembly, 1, ASSEMBLY_SIZE, file);
		fclose(file);
	} else {
		printf("Error: Could not open file %s\n", filename);
	}
	
}

void parseLine(struct token *line) {
	line->token = dstring_remove(line->token, '\t');
	struct token *tokens = token_tokenize(line->token, ' ');

	printf("%s\n", line->token->string);

	token_free(tokens);
}

int assemble(const char *filename) {
	readAssembly(filename);
	struct dstring *string = dstring_new(assembly);
	struct token *token = token_tokenize(string, '\n');

	token_each(token, parseLine);
	
	token_free(token);
	dstring_free(string);
	return 0;
}

int main(int argc, char *argv[]) {
    assemble(argv[1]);
	return 0;
}