#include<string>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

#define SIZE 2000

using namespace std;
#define handle_error(msg) \
	do{ perror(msg);}while(0);

#include "shell.h"

void run(int command_number){
	char * options[command_options[command_number]];
	int i=0,j;
	for(i=0;i<command_options[command_number];i++){
		options[i] = (char *) malloc(SIZE * sizeof(char));
	}
	for(i=0,j=0;i<command_options[command_number];i++){
		if(command_parts[command_number][i].compare("<")==0 || command_parts[command_number][i].compare(">")==0 || command_parts[command_number][i].compare("&")==0) {
			i++;
			continue;
		}
		strcpy(options[j],command_parts[command_number][i].c_str());
		j++;
	}
	options[j]=NULL;
	
	int status_execvp = execvp(options[0],options);
	if(status_execvp<0) {
		handle_error("execvp threw error");
	}
}