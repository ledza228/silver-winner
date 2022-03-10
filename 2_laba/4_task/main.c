#include <stdio.h>
#include <dirent.h>


int print_directory(char const *dir_name){
    DIR *current_dir = opendir(dir_name);
    if(!current_dir){
        perror("opendir");
        return 1;
    }

    struct dirent *d;

    while((d = readdir(current_dir))){
        printf("%s\n", d->d_name);
    }

    if(closedir(current_dir)){
        perror("closedir");
        return 1;
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    printf("Current directory:\n");
    print_directory("./");
    printf("---------------------\n");
    printf("Root directory:\n");
    print_directory("/");

    return 0;

}
