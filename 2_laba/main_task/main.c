#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h> 

#include "stack.c"

struct stack_t dir_stack;
FILE* outputFile;

int isEmptyString(char* name){
	if (name[0] == '/')
		return 0;
	if (name[0] == '\n')
		return 0;
	return name[0];
}


int isNotValidDirectory(char* dirName){
	if (!isEmptyString(dirName)){
		return 1;
	}
	if (strcmp(dirName,".") == 0 || strcmp(dirName,"..") == 0){
		return 1;
	}
	return 0;
}


char* addSlashToDirName(char* dirName){
	if (dirName[strlen(dirName)-1] == '/'){
		return dirName;
	}
	else
		return strcat(dirName, "/");
}



int getFileSize(char* fileName){
	char tmp[255];
	strcpy(tmp, fileName);
	struct stat buff;
	int res = stat(tmp, &buff);
	if (res != 0){
		fprintf(stderr,"error get file properties - %s", fileName);
		fprintf(outputFile,"error get file properties - %s", fileName);
	}
	return buff.st_size;
}

char* getFileName(struct dirent* file){
	return file->d_name;
}

int new_isDirectory(char* fileName){
	char tmp[255];
	strcpy(tmp, fileName);

	struct stat buff;
	int errHandle = stat(tmp, &buff);
	if (errHandle != 0){
		fprintf(stderr, "error get file properties - %s", fileName);
		fprintf(outputFile, "error get file properties - %s", fileName);
	}
	if (S_ISDIR(buff.st_mode)){
		return 1;
	}
	return 0;
}

void processDirectory(char* dirName){

	long long size = 0;
	int amntOfFiles = 0;

	int maxSizeFile = 0;
	char maxSizeFileName[255] = "";


	DIR* dir = opendir(dirName);
	if (dir == NULL){
		fprintf(stderr,"Failed to open directory - %s", dirName);
		fprintf(outputFile,"Failed to open directory - %s", dirName);
		exit(EXIT_FAILURE);
	}

	struct dirent* dirFile;
	while ((dirFile = readdir(dir)) != NULL){
		
		char* name = dirFile->d_name;
		
		char* fullPath = malloc(256);
		fullPath = strcpy(fullPath, dirName);
		strcat(fullPath, name);

		if (new_isDirectory(fullPath) == 1){
			if (isNotValidDirectory(name)){
				free(fullPath);
				continue;
			}
		
			fullPath = addSlashToDirName(fullPath);
			push(&dir_stack, fullPath);
			free(fullPath);
			continue;
		}
		
		int fileSize = getFileSize(fullPath);
		if (fileSize > maxSizeFile){
			strcpy(maxSizeFileName, fullPath);
			maxSizeFile = fileSize;
		}

		size += fileSize;	
		amntOfFiles++;
		free(fullPath);
	}
	
	printf("%s:\n", dirName);
	printf("amount of files: %d\n", amntOfFiles);
	printf("total size: %d bytes\n", size);
	printf("biggest file: %s - %d bytes\n\n", maxSizeFileName, maxSizeFile);

	fprintf(outputFile,"%s:\n", dirName);
	fprintf(outputFile,"amount of files: %d\n", amntOfFiles);
	fprintf(outputFile,"total size: %d bytes\n", size);
	fprintf(outputFile,"biggest file: %s - %d bytes\n\n", maxSizeFileName, maxSizeFile);

	int err = closedir(dir);
}

void process(char* directory){
	directory = addSlashToDirName(directory);
	push(&dir_stack, directory);
	
	while (!isEmpty(&dir_stack)){
		char* dir_name = pop(&dir_stack);
		processDirectory(dir_name);
		free(dir_name);
	}

}

int main(int argc, char *argv[]){

	if (argc < 3){
		fprintf(stderr, "ERROR: not enough params\n");
		fprintf(stderr, "usage: %s <dir> <out_file>", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	char* directory = argv[1];
	char* output_file = argv[2];

	outputFile = fopen(output_file, "wr+");
	if (outputFile == NULL){
		fprintf(stderr, "can't open/create file - %s", output_file);
		exit(EXIT_FAILURE);
	}

	process(directory);
	
	fclose(outputFile);
	return 0;
}
