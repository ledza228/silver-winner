#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	if (argc < 3){
		printf("usage: %s <src_file> <dist_file>", argv[0]);
		return EXIT_FAILURE;
	}

	FILE* file_1 = fopen(argv[1], "r");

	if (file_1 == NULL){
		printf("Error: can't open file %s", argv[1]);
		return EXIT_FAILURE;
	}

	FILE* file_2 = fopen(argv[2], "w");

	if (file_2 == NULL){
		printf("Error: can't open/create file %s", argv[2]);
		return EXIT_FAILURE;
	}


	char symb;
	while ((symb = fgetc(file_1)) != EOF){
		fputc(symb, file_2);
	}

	struct stat copy_stat;
	stat(argv[1], &copy_stat);

	chmod(argv[2], copy_stat.st_mode);

	fclose(file_1);
	fclose(file_2);
}
