#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define TRUE 1
#define OFFSET 10

// char *command[] = {"cd","exit"};

char **path;
int path_num = 0;
int path_size = 10;
int i = 0;
int single_path_len =30;
char line[256]; //user input
char *token;
int status = -1;
char *cmdArgv[5]={NULL, NULL, NULL, NULL, NULL};
int cmdArgc = 0;
char *cmd = NULL;
char *dir = NULL;

void type_prompt(){
	printf("$ ");
}
void init(){
	errno = 0; 
	path = malloc( path_size * sizeof(char*) );
	for(i = 0; i < path_size; i++){
		path[i] = malloc( single_path_len * sizeof(char));
	}
	strcpy(path[0], "/bin");
	path_num++;
}

void reset_for_loop(){
	for(i=0; i<5; i++){
		cmdArgv[i] = NULL;
	}

	cmdArgc = 0;
	cmd = NULL;
	dir = NULL;
}

void read_and_parse_input(){
	fgets(line, sizeof(line), stdin);
	//cancel the last \n
	char *tail = NULL;
	if((tail = strchr(line, '\n')) != NULL)
		*tail = '\0';

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
}

void manage_path() {

	dir = cmdArgv[2];

	//null pointer, print path
	if(!cmdArgv[1]) {
		printf("%s", path[0]);

		for(i = 1; i < path_num; i++){
			printf(":%s", path[i]);
		}
		printf("\n");

	//path + dir
	} else if(!strcmp(cmdArgv[1], "+")){
		//reallocate memory
		if(path_num >= path_size){
			char ** bigger_path = realloc( path, path_size + OFFSET);
			if(bigger_path == NULL){}
				//error
			else{
				path = bigger_path;

				for(i = path_size; i < path_size + OFFSET; i++){
					path[i] = malloc( single_path_len * sizeof(char));
				}
				path_size += OFFSET;
			}
		}

		//add to path
		if(dir == NULL){}
			//error
		else{
			strcpy(path[path_num], dir);
			path_num++;
		}

	//path - dir
	} else if(!strcmp(cmdArgv[1], "-")){
		//no such dir

		// char *pathv[]

		// //split path into array
		// token = strtok(path, ":");
		// if(token == NULL) //no colon - no dir or 1 dir
		// 	cmdArgv[0] = line;

		// while(token != NULL){
		// 	cmdArgv[cmdArgc] = token;
		// 	token = strtok(NULL, " ");
		// 	cmdArgc++;
		// }
		// if(!strstr(path, dir)){
		// 	printf("error: %s\n", strerror(errno));
		// }
		// else {

		// }
	}
}

int main(int argc, char **argv) {
	// char *command[]={"exit"};
	
	init();

	while (TRUE) {
		//revert cmdargv
		reset_for_loop();

		type_prompt();

		//read input
		read_and_parse_input();
		
		//exit
		if(!strcmp(cmd, "exit"))
			exit(0);

		//path
		else if(!strcmp(cmd, "path")){
			manage_path();
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
