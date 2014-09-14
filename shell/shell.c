#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define TRUE 1

// char *command[] = {"cd","exit"};
char *cmdArgv[5]={NULL, NULL, NULL, NULL, NULL};
int cmdArgc = 0;
char *cmd = NULL;
char *path;
int path_len = 0;


void type_prompt(){
	printf("$ ");
}
void init(){
	char *init_path = "/bin";
	path_len = strlen(init_path);
	path = (char*)malloc(strlen(init_path) * sizeof(char));
	strcpy(path, init_path);
	printf("%d\n", path_len);
}

int main(int argc, char **argv) {
	// char *command[]={"exit"};
	char line[256]; //user input
	char *token;
	int status = -1;
	// int i;

	init();

	while (TRUE) {
		type_prompt();

		//read input
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
		if(!strcmp(cmd, "exit"))
			exit(0);

		//path
		else if(!strcmp(cmd, "path")){
			//null pointer, print path
			if(!cmdArgv[1]) {
				printf("%s\n", path);
			//+ - path
			} else if(!strcmp(cmdArgv[1], "+")){
				//reallocate memory
				path_len += strlen(cmdArgv[2]) + 1; //1 for colon
				path = (char*)realloc(path, path_len * sizeof(char));
				strcat(path, ":");
				strcat(path, cmdArgv[2]);
				printf("%s\n", path);
				printf("%d\n", path_len);
			} else if(!strcmp(cmdArgv[1], "-")){

			}
		}

		//ls
		else if(strcmp(cmd, "ls") == 0){
			printf("reach ls");
			if(fork() != 0){
				/* Parent */
				wait(&status);
				printf("\n");

			} else{
				/* Child*/
				// execv(file_name, cmdArgv);
				exit(0);
			}
		}

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
