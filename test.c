#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#define MAXL 4096


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
	return token;
}

void execute_line(char *line)
{
	char** token = tokenize(line);
}

int main()
{
	char *line, *s;
	char cmd[MAXL], param[10][MAXL];

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

