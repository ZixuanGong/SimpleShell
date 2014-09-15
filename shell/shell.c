#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define TRUE 1
#define OFFSET 10

char **path;
size_t path_num = 0;
size_t path_size = 10;
size_t single_path_len =30;

char **cmdArgv;
size_t cmdArgc = 0;
size_t cmdArgSize = 10;

int status = -1;

void print_error(char *msg)
{
	if( msg == NULL )
		printf("error: %s\n", strerror(errno));
	else
		printf("error: %s\n", msg);
}

void init()
{
	errno = 0;

	path = malloc(path_size * sizeof(char*));
	int i;
	for (i = 0; i < path_size; i++)
		path[i] = malloc(single_path_len * sizeof(char));
	strcpy(path[0], "/bin");
	path_num++;

}

void read_and_parse_input(){
	char *line;
	size_t n = 128;
	if (getline(&line, &n, stdin) == -1) {
		print_error("Error in getline()");
		return;
	}
	/* cancel the last \n */
	char *tail = NULL;
	if ((tail = strchr(line, '\n')) != NULL)
		*tail = '\0';
	/* split input */
	char *token = strtok(line, " ");
	while (token != NULL && cmdArgc <= 10) {
		cmdArgv[cmdArgc] = token;
		token = strtok(NULL, " ");
		cmdArgc++;
	}
}

int find_position(char **array, size_t num, char *string)
{
	int i;
	for (i = 0; i < num; i++) {
		if (strcmp(array[i], string) == 0)
			return i;
	}
	return -1;
}


void enlarge_path_size()
{
	char **bigger_path = realloc(path, path_size + OFFSET);
	if (bigger_path == NULL) {
		print_error("Error in enlarge_path_size()");
	}
	else {
		path = bigger_path;
		int i;
		for (i = path_size; i < path_size + OFFSET; i++)
			path[i] = malloc(single_path_len * sizeof(char));
		path_size += OFFSET;
	}
}

void manage_path()
{
	char *dir = cmdArgv[2];

	/* path */
	if (cmdArgc == 1) {
		printf("%s", path[0]);
		int i = 0;
		for(i = 1; i < path_num; i++)
			printf(":%s", path[i]);
		printf("\n");
	}
	/* path + */ 
	else if (strcmp(cmdArgv[1], "+") == 0 && dir != NULL) {
		if (find_position(path, path_num, dir) >= 0) {
			print_error("Directory already exists");
			return;
		}
		if (path_num >= path_size) 
			enlarge_path_size();
		strcpy(path[path_num], dir);
		path_num++;
	}
	/* path - */ 
	else if (strcmp(cmdArgv[1], "-") == 0 && dir != NULL) {
		if(path_num == 0){
			print_error("The path is empty!");
			return;
		}

		int pos = find_position(path, path_num, dir);

		if (pos == -1) {
			print_error("No such directory in the path!");
			return;
		}

		if (path_num == 1) {
			path[0] = NULL;
		} else {
			path[pos] = path[path_num - 1];
			path[path_num - 1] = NULL;
		}
		path_num--;
	} else {
		print_error("Invalid command");
	}
}

void change_directory(){

}

void exec_command()
{
	char *file_name = malloc(128 * sizeof(char));
	strcpy(file_name, "/bin/");
	strcat(file_name, cmdArgv[0]);

	if(fork() != 0){
		wait(&status);

	} else{
		execv(file_name, cmdArgv);
		exit(0);
	}
}

int main(int argc, char **argv)
{
	char *commands[] = {"cd", "exit", "path"};
	cmdArgv = malloc(cmdArgSize * sizeof(char*));;
	init();

	while (TRUE) {
		cmdArgc = 0;
		int i;
		for (i = 0; i < cmdArgSize; i++)
			cmdArgv[i] = NULL;

		printf("$ ");
		read_and_parse_input();
		switch (find_position(commands, 3, cmdArgv[0])) {
		case 0:
			change_directory();
			break;
		case 1:
			exit(0);
			break;
		case 2:
			manage_path();
			break;
		default:
			exec_command();
			break;
		}

		// else if(strcmp(cmd, "ls") == 0){
		// 	printf("reach ls");
		// 	if(fork() != 0){
		// 		/* Parent */
		// 		wait(&status);
		// 		printf("\n");

		// 	} else{
		// 		/* Child*/
		// 		// execv(file_name, cmdArgv);
		// 		exit(0);
		// 	}
		// }

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
