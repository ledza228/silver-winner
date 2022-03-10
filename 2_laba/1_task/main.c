#include<stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	if (argc < 2){
		printf("usage: %s <file_name>", argv[0]);
		return EXIT_FAILURE;
	}

	char* file_name = argv[1];

	FILE* file = fopen(file_name, "w");

	if (file == NULL){
		printf("Error: can't open file %s", file_name);
		return EXIT_FAILURE;
	}

	char symb;
	while ((symb = getc(stdin)) != 6){
		fputc(symb, file);
	}

	fclose(file);
}
