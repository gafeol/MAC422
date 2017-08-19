#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define BUFLEN 4096

#define STDIN   0
#define STDOUT  1

int main (int argc, char *argv[]){
	int pipe_arr[2];
  char buf[BUFLEN];

	if(strcmp(argv[1], "date") == 0){
		time_t result = time(NULL);
		if(result != -1){
			printf("%s", asctime(localtime(&result)));
		}
	}
	else if(strcmp(argv[1], "/bin/ping") == 0){
		pipe(pipe_arr);
		if (fork() == 0){
		    //int err;
		    //char *env[1] = { "www.google.com.br" };
		    //char *argv[3] = {"ping"};
				//err = execve("/bin/ping", argv, env);  //syscall, libc has simpler wrappers (man exec)
		    //exit(err); //if it got here, it's an error
				dup2(pipe_arr[1], STDOUT);
				execl("/bin/ping", "ping", "-c 10", "www.google.com.br", (char*)NULL);
		}
		else {
        wait(NULL);
        read(pipe_arr[0], buf, BUFLEN);
        printf("%s\n", buf);
    }

		close(pipe_arr[0]);
    close(pipe_arr[1]);
	}
}
