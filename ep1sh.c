#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

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
			dup2(pipe_arr[1], STDOUT);
			execl("/bin/ping", "ping", argv[2], argv[3], argv[4], (char*)NULL);
		}
		else {
			wait(NULL);
			read(pipe_arr[0], buf, BUFLEN);
			printf("%s\n", buf);
		}

		close(pipe_arr[0]);
		close(pipe_arr[1]);
	}
	else if(strcmp(argv[1], "/usr/bin/cal") == 0){
		pipe(pipe_arr);
		if (fork() == 0){
			dup2(pipe_arr[1], STDOUT);
			execl("/usr/bin/cal", "cal", argv[2], (char*)NULL);
		}
		else {
			wait(NULL);
			read(pipe_arr[0], buf, BUFLEN);
			printf("%s\n", buf);
		}

		close(pipe_arr[0]);
		close(pipe_arr[1]);
	}
	else if(strcmp(argv[1], "./ep1") == 0){
		pipe(pipe_arr);
		if (fork() == 0){
			dup2(pipe_arr[1], STDOUT);
			// compila o ep1 com um argumento (argv[2])
			execl("./ep1", "ep1", argv[2], (char*)NULL);
		}
		else {
			wait(NULL);
			read(pipe_arr[0], buf, BUFLEN);
			printf("%s\n", buf);
		}

		close(pipe_arr[0]);
		close(pipe_arr[1]);
	}
	else if(strcmp(argv[1], "chown") == 0){
		struct group  *grp;
		// remove o primeiro elemento da string (tira o : de :group)
		memmove (argv[2], argv[2]+1, strlen (argv[2]+1) + 1); 
		grp = getgrnam(argv[2]);
		if (grp == NULL) {
			printf("Failed to get gid");
		}
		gid_t gid = grp->gr_gid;
		if(chown(argv[3], -1, gid) == -1){
			printf("chown fail");
		}
	}
}
