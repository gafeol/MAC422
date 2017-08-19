#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int main (int argc, char *argv[])
{
	if(strcmp(argv[1], "date") == 0){
		time_t result = time(NULL);
		if(result != -1){
			printf("%s", asctime(localtime(&result)));
		}
	}
	else if(argv[1][0] == '/'){
		int pid = fork();
		if (pid=0){
		    int err;
		    char *env[1] = { 0 };
		    char *argv[3] = { "ls", "-l", 0 };
				err = execve("/bin/ls", argv, env);  //syscall, libc has simpler wrappers (man exec)
		    exit(err); //if it got here, it's an error
		} else if(pid<0) {
		    printf("fork failed with error code %d\n", pid);
		    exit(-1);
		}

		int status;
		wait(&status); //simplest one, man wait for others
		printf("child pid was %d, it exited with %d\n", pid, status);
		exit(0);
	}
}
