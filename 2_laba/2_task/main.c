#include <stdio.h>
#include <stdlib.h>



int main (int argc, char *argv[]){
	

	if (argc < 3){
		printf("usage: %s <file_name> <amount_of_output_strings>", argv[0]);
		return EXIT_FAILURE;
	}

	char * file_name = argv[1];
	
	char* setting_string = argv[2];
	int setting = atoi(setting_string);

	
	FILE* file = fopen(file_name, "r");
	
	if (file == NULL){
		printf("Error: can't open file %s", argv[1]);
		return EXIT_FAILURE;
	}


	char symb;
	int amnt_of_lines = 0;
	while ((symb = fgetc(file)) != EOF){
		if (symb == '\n')
			amnt_of_lines++;
		if (amnt_of_lines == setting && setting != 0){
			getc(stdin);
			amnt_of_lines = 0;
		}

		putc(symb, stdout);

	}
	fclose(file);

}
