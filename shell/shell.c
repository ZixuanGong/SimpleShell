#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define TRUE 1

// char *command[] = {"cd","exit"};
char *cmdArgv[5];
int cmdArgc = 0;
char *cmd = NULL;

void read_command() {
	char line[256]; //user input
	char *token;
	
	fgets(line, sizeof(line), stdin);

	//split line
	token = strtok(line, " ");
	if(token == NULL) //no white space
		cmdArgv[0] = line;

	while(token != NULL){
		cmdArgv[cmdArgc] = token;
		token = strtok(NULL, " ");
		cmdArgc++;
	}
	cmd = cmdArgv[0];
	printf("%s\n", cmd);
}

void type_prompt(){
	printf("$ ");
}

int main(int argc, char **argv) {
	// char *command[]={"exit"};
	char *cmdArgv[5];
	int cmdArgc = 0;
	char line[256]; //user input
	char *token;
	char *cmd;

	while (TRUE) {
		type_prompt();

		//read input
		fflush(stdout);
		fgets(line, sizeof(line), stdin);
		//cancel the last \n
		char *p = NULL;
		if((p = strchr(line, '\n')) != NULL)
			*p = '\0';

		//split line
		token = strtok(line, " ");
		if(token == NULL) //no white space
			cmdArgv[0] = line;

		while(token != NULL){
			cmdArgv[cmdArgc] = token;
			token = strtok(NULL, " ");
			cmdArgc++;
		}
		cmd = cmdArgv[0];

		//exit
		if(strcmp(cmd, "exit") == 0)
			exit(0);

		// printf("%d\n", t);
		// printf("%s\n", cmd);


		// if(fork() != 0){
		// 	/* Parent */
		// 	waitpid(-1, &status, 0);

		// } else{
		// 	/* Child*/
		// 	execve(cmd, params, 0);
		// }
	
	}
	return 0;
}
