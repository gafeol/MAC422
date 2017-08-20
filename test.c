#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#define MAX_LENGTH 1024


void type_prompt()
{
	//alteracoes do susu
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)

		printf("[%s]$ ", cwd);
	else
		perror("getcwd() error");
//	printf("[%s]$ ", get_current_dir_name());
}

int main()
{
	char *line;
	char cmd[MAX_LENGTH], param[10][MAX_LENGTH];

	while(1) {
		type_prompt();
		line = readline("");

		if(!line) break;

		s = stripwhite(line);

		if(*s) {
			add_history(line);
			//execute_line(line);
		}
		//read_command(cmd,param);	
	}
	return 0;
}

