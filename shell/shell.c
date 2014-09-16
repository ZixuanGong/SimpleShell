#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define TRUE 1
#define OFFSET 10
#define MAX_CMD_ARGS 10

char **path;
size_t path_num = 0;
size_t path_size = 10;
size_t single_path_len =30;

char **cmdArgv;
size_t cmdArgc = 0;

void print_error(char *msg)
{
	if( msg == NULL )
		printf("error: %s\n", strerror(errno));
	else
		printf("error: %s\n", msg);
}

void init()
{
	int i;
	errno = 0;

	path = malloc(path_size * sizeof(char*));
	for (i = 0; i < path_size; i++)
		path[i] = malloc(single_path_len * sizeof(char));
	strcpy(path[0], "/bin");
	path_num++;
}

void read_and_parse_input(){
	char *line = NULL;
	size_t n = 128;
	char *tail = NULL;
	char *token = NULL;

	if (getline(&line, &n, stdin) == -1) {
		print_error("error in getline()");
		return;
	}
	/* cancel the last \n */
	if ((tail = strchr(line, '\n')) != NULL)
		*tail = '\0';
	/* split input */
	token = strtok(line, " ");
	while (token != NULL && cmdArgc <= MAX_CMD_ARGS) {
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
	int i;
	char **bigger_path = realloc(path, path_size + OFFSET);
	if (bigger_path == NULL) {
		print_error("error in enlarge_path_size()");
	}
	else {
		path = bigger_path;
		for (i = path_size; i < path_size + OFFSET; i++)
			path[i] = malloc(single_path_len * sizeof(char));
		path_size += OFFSET;
	}
}

void manage_path()
{
	char *dir = cmdArgv[2];
	int i = 0;
	int pos;

	/* path */
	if (cmdArgc == 1) {
		printf("%s", path[0]);
		for(i = 1; i < path_num; i++)
			printf(":%s", path[i]);
		printf("\n");
	}
	/* path + */ 
	else if (strcmp(cmdArgv[1], "+") == 0 && dir != NULL) {
		if (find_position(path, path_num, dir) >= 0) {
			print_error("directory already exists");
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
			print_error("the path is empty");
			return;
		}

		pos = find_position(path, path_num, dir);

		if (pos == -1) {
			print_error("no such directory in the path");
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
		print_error("invalid command");
	}
}

void change_directory(){
	char *file_name = malloc(128 * sizeof(char));
	if (cmdArgv[1] != NULL)
		strcpy(file_name, cmdArgv[1]);
	else
		file_name = "/home";
	if (chdir(file_name) == -1)
		print_error(NULL);
}

void exec_command()
{
	int status;
	char *file_name = malloc(128 * sizeof(char));
	strcpy(file_name, "/bin/");
	strcat(file_name, cmdArgv[0]);

	if(fork() != 0){
		wait(&status);

	} else{
		if (execv(file_name, cmdArgv) == -1) {
			print_error("command not found");
			_exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char **argv)
{
	int i;
	int pos;
	char *commands[] = {"cd", "exit", "path"};
	cmdArgv = malloc(MAX_CMD_ARGS * sizeof(char*));;
	init();

	while (TRUE) {
		for (i = 0; i < MAX_CMD_ARGS; i++)
			cmdArgv[i] = NULL;
		cmdArgc = 0;

		printf("$ ");
		read_and_parse_input();
		pos = find_position(commands, 3, cmdArgv[0]);
		switch (pos) {
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
	}
	return 0;
}
