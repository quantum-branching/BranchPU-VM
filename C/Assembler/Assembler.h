#include <stdio.h>
#include "Preprocessor.c"

#define ASSEMBLY_SIZE 65536

char assembly[ASSEMBLY_SIZE];

void readAssembly(const char *filename);

struct token *removeComments(struct token *lines);

int assemble(const char *filename);

int main(int argc, char *argv[]);