#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <wait.h>


int main(){

	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	struct tm* time = localtime(&ts.tv_sec);

	printf("(parrent process):\t%d -- pid\t%d -- parrent pid\ttime -- %d:%d:%d:%d \n\n",
						getpid(), getppid(), time->tm_hour, time->tm_min, time->tm_sec, (int) ts.tv_nsec /1000000);

	pid_t pid;
	for (int i = 0; i<2;i++){
		pid = fork();

		if (pid == 0){
			
			struct timespec ts;
			clock_gettime(CLOCK_REALTIME, &ts);

			struct tm* time = localtime(&ts.tv_sec);

			printf("(%d child process):\t%d -- pid\t%d -- parrent pid\ttime -- %d:%d:%d:%d \n\n",
						i+1, getpid(), getppid(), time->tm_hour, time->tm_min, time->tm_sec, (int) ts.tv_nsec /1000000);

			exit(EXIT_SUCCESS);

		}
		else if (pid < 0){
			perror("ERROR CREATING CHILD PROCESS");
			exit(EXIT_FAILURE);
		}
	}

	if (pid > 0){
		system("ps -x");
		while(wait(NULL) != -1);
	}
	

}
