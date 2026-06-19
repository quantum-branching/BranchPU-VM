#include <stdio.h>
#include "Preprocessor.c"

#define ASSEMBLY_SIZE 65536

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

struct token *removeComments(struct token *lines) {
	if(lines == NULL) {
		return NULL;
	}

	struct dstring *contents = lines->token;
	struct dstring *left = DSTRING_EMPTY;
	struct dstring *right = DSTRING_EMPTY;

	dstring_split(contents, ';', left, right);

	lines->token = dstring_tolower(left);
	lines->next = removeComments(lines->next);
	return lines;
}

int assemble(const char *filename) {
	enterArena(ASSEMBLY_SIZE);

	readAssembly(filename);
	struct dstring *string = dstring_new(assembly);
	string = dstring_remove(string, '\t');
	string = dstring_remove(string, '\r');
	struct token *lines = token_tokenize(string, '\n');

	lines = removeComments(lines);
	parseDirective(lines, lines);

	exitArena();
	return 0;
}

int main(int argc, char *argv[]) {
	clock_t start = clock();
	
	for(int i = 0; i < 1; i++) {
		assemble(argv[1]);
	}

	printf("%f\n", (float)(clock() - start) / CLOCKS_PER_SEC);
	return 0;
}