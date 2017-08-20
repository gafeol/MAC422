#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#define MAX_LENGTH 1024


/*char type_prompt()
{
	//alteracoes do susu
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		return cwd;
	else {
		perror("getcwd() error");
	}

//	printf("[%s]$ ", get_current_dir_name());
}*/

int main()
{
	char *line;
	char cmd[MAX_LENGTH], param[10][MAX_LENGTH];

	while(1) {
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) == NULL)
			perror("getcwd() error");
		line = readline(cwd);

		if(!line) break;

		//s = stripwhite(line);

		if(*line) {
			add_history(line);
			//execute_line(line);
		}
		//read_command(cmd,param);	
	}
	return 0;
}

