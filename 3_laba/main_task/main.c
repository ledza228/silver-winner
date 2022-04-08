#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <wait.h>
#include <math.h>
#include <errno.h>
#include <string.h>



double getSumFromFile(FILE* file, int n){
    
    double sum = 0;

    for (int i = 0; i<n; i++){
        float number;
        int f;
        int temp;

        int isEnd = fscanf(file, "%d pid %f\n", &f, &number, &temp);

        sum += number;
    }

    return sum;
}


double sinTaylor(double xVal, int rowMemberNumber){

    return pow(-1, rowMemberNumber) * (pow(xVal,2 * rowMemberNumber + 1)) / tgamma((2 * rowMemberNumber + 1) + 1);
}


int main(int argc, char const *argv[])
{

    if (argc < 3){
        fprintf(stderr, "usage: %s <amount_lines> <amount_members_in_taylor>\n", argv[0], strerror(errno));
        return EXIT_FAILURE;
    }

    char *endptr, *first_arg = argv[1];

    errno = 0;
    int N = (int) strtol(first_arg, &endptr, 10);
    if (errno != 0){
        fprintf(stderr, "Error: number is uncorrect %s\n", argv[1], strerror(errno));
        return EXIT_FAILURE;
    }

    if ((first_arg == endptr) || (endptr - argv[2] < strlen(first_arg)) || (N >= 5000))
    {
        fprintf(stderr, "first param must be integer! and lower than 5000\n");
        return EXIT_FAILURE;
    }

    char *endptr2, *first_arg2 = argv[2];

    errno = 0;
    int n = (int) strtol(first_arg2, &endptr2, 10);
    if (errno != 0){
        fprintf(stderr, "Error: number is uncorrect %s\n", argv[2], strerror(errno));
        return EXIT_FAILURE;
    }

    if ((first_arg == endptr2) || (endptr2 - argv[2] < strlen(first_arg2)) || (n >= 20))
    {
        fprintf(stderr, "first param must be integer! and lower than 20\n");
        return EXIT_FAILURE;
    }


   FILE* file = fopen("res_file.txt", "w+t");

    if (file == -1){
        perror("Can't create file");
        exit(EXIT_FAILURE);
    }

    int currentLine = 0;

    for (int i = 0; i< N;i++){

        double xVal = 2 * M_PI * i / N;

        pid_t pid;
        for (int j = 0; j<n; j++){
            
            pid = fork();
            if (pid == 0){
                
                double res = sinTaylor(xVal, j);
                
                printf("%d pid %f\n", getpid(), res);
                fprintf(file, "%d pid %f\n", getpid(), res);
                fflush(file);

                exit(EXIT_SUCCESS);
            }
            else if ( pid < 0){
                perror("Error creating child process");
            }
        }

        while(wait(NULL) != -1);
        fflush(file);

        int temp = fseek(file, currentLine, SEEK_SET);
        double sumJ = getSumFromFile(file, n);    
        
        printf("y[%d] -- %f\n", i, sumJ);
        fprintf(file, "y[%d] -- %f\n", i, sumJ);
        
        fflush(file);
        
        currentLine = ftell(file);
        
    }

    return 0;
}
