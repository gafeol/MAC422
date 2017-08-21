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
#include <readline/readline.h>
#include <readline/history.h>

#define BUFLEN 8192

#define STDIN   0
#define STDOUT  1

#define MAXL 1024

char* createprompt()
{
	char *dir, *prompt;

	dir = malloc(MAXL*sizeof(char));
	prompt = malloc(MAXL*sizeof(char));

	if(!dir || !prompt) {
		fprintf(stderr,"erro de alocacao\n");
		exit(EXIT_FAILURE);
	}

	if(getcwd(dir,MAXL*sizeof(char)) == NULL) {
		fprintf(stderr,"erro em getcwd\n");
		exit(EXIT_FAILURE);
	}

	strcpy(prompt,"[");
	strcat(prompt,dir);
	strcat(prompt,"]$ ");

	free(dir);
	return prompt; 
}

char* stripwhite(char *string)
{
	char *s, *t;

	for(s = string; whitespace(*s); s++);

	if(*s == '\0') return s;

	t = s + strlen(s) - 1;
	while(t > s && whitespace(*t)) {
		t--;
	}
	*(++t) = '\0';

	return s;
}

char** tokenize(char *line)
{
	char** token = malloc(MAXL*sizeof(char*));
	int cnt = 0;
	if(token == NULL) {
		fprintf(stderr,"erro de alocacao\n");
		exit(EXIT_FAILURE);
	}
	char *p;
	p = strtok(line," ");
	while(p != NULL) {
		token[cnt++] = p;
		p = strtok(NULL," ");
	}
	token[cnt] = 0;
	return token;
}

void execute_line(char *line)
{
	pid_t pid;
	char** token = tokenize(line);
	int pipe_arr[2];
	char *buf = malloc(BUFLEN*sizeof(char));

	if(strcmp(token[0], "date") == 0){
		time_t result = time(NULL);
		if(result != -1){
			printf("%s", asctime(localtime(&result)));
		}
	}
	else if(strcmp(token[0], "chown") == 0){
		struct group  *grp;
		// remove o primeiro elemento da string (tira o : de :group)
		memmove (token[1], token[1]+1, strlen (token[1]+1) + 1); 

		grp = getgrnam(token[1]);
		if (grp == NULL) {
			printf("Failed to get gid");
		}
		gid_t gid = grp->gr_gid;
		if(chown(token[2], -1, gid) == -1){
			printf("chown fail");
		}
	}
	else{
		int status;
		switch (pid = fork()) {
			case -1:
				perror("fork()");
				exit(EXIT_FAILURE);
			case 0: 
				status = execve(token[0], token, 0);
				exit(status);
			default: 
				if ((waitpid(pid, &status, 0)) < 0) {
					perror("waitpid()");
					exit(EXIT_FAILURE);
				}
		}
	}
	free(buf);
	free(token);
}

int main()
{
	char *line, *s;
	using_history();

	while(1) {
		char *prompt;
		prompt = createprompt();
		if(prompt == NULL) {
			printf("error\n");
			break;
		}
		line = readline(prompt);
		free(prompt);
		if(!line) break;
		s = stripwhite(line);
		if(*s) {
			add_history(s);
			execute_line(s);
		}
		free(line);
		//read_command(cmd,param);	
	}
	return 0;
}
