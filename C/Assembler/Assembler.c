#include "Assembler.h"

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
	struct dstring *left;
	struct dstring *right;

	dstring_split(contents, ';', left, right);
	dstring_tolower(left);

	lines->token = left;
	lines->next = removeComments(lines->next);
	return lines;
}

int assemble(const char *filename) {
	readAssembly(filename);

	struct dstring string = dstring(assembly);
	struct token *lines;

	dstring_remove(&string, '\t');
	dstring_remove(&string, '\r');
	// token_tokenize(&string, lines,'\n');
	debug2("token_tokenize(%p, %p, \'\\n\')\n", &string, lines);

	lines = removeComments(lines);
	debug("removeComments(%p)\n", lines);
	// parseDirective(lines, lines);

	return 0;
}

int main(int argc, char *argv[]) {
	assemble(argv[1]);
	return 0;
}