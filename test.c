#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
	char line[MAX_LENGTH];
	char cmd[MAX_LENGTH], param[10][MAX_LENGTH];

	while(1) {
		type_prompt();
		scanf("%s", cmd);
		//read_command(cmd,param);	
	}
	return 0;
}

