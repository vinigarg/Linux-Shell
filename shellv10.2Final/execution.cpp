#include <string>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <stdio.h>

#define handle_error(msg) \
	do{ perror(msg);}while(0);

using namespace std;

#include "shell.h"

stack <int> backgroundStack;

/************Select which command to run*****************/
void selection(int i){
	if(command_parts[i][0].compare("pwd")==0){
		if(getCWD()<0) handle_error("pwd not working");
		cout<<endl;
	}else if(command_parts[i][0].compare("echo")==0){
		if(echo(i)==-1) {
			handle_error("echo command invalid");
		}
	}else if(command_parts[i][0].compare("history")==0){
		viewHistory(i);
	}else{
		int stat = checkForLocalAssignment(i,0);
		if(stat==-1) {
			run(i);
		}
	}
	exit(0);	
}


void process_creation(int in, int out,int cmd_no){
  	pid_t pid = fork ();
	
  	if (pid == 0){	
	    if (in != 0){	
    	    dup2 (in, 0);
        	close (in);
    	}	
    	if(out != 1){	
        	dup2 (out, 1);
        	close (out);    
    	}
		selection(cmd_no);
    }
}

void forking(){
	int i;
	
	int in, out,fd [2];
	in = 0;
	/*******input redirections ******************************/	
	if(mode1 ==1){
		//cout<<inputFile<<endl;
		int inputfd = open(inputFile.c_str(),O_RDONLY ,  S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		in = inputfd;
	}  

	/*******calling process ******************************/
	for(i=0;i< number_of_commands-1; ++i){
		pipe(fd);
		process_creation(in, fd [1],i);
		close (fd [1]);
		in = fd [0];
	}
	if (in != 0) dup2 (in, 0);
	
	/*******output redirections ******************************/
	if (mode2 == 1){
		int outputfd = open(outputFile.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		if(outputfd==-1) handle_error("FD : ");	
		dup2 (outputfd, 1);
	}      
	selection(i);
}


/********************void execute*******************/
void  execute(){
    pid_t  pid;
    int  status;
    pid = fork();
    if(mode3==1){
	    if(pid < 0) {     
    	    handle_error("fork threw error\n");
        	return;
   		}
   		else if(pid == 0) forking();
   		else{
    	//insert pid in queue
   			cout<<"["<<pid<<"]"<<endl;
    		backgroundStack.push(pid);	
    	}
    }else{
    	if(pid < 0) {     
    	   	handle_error("fork threw error\n");
        	return;
   		}
    	else if(pid == 0) forking();
    	else{
	    	do{
	    		pid_t wpid = waitpid(pid,&status,WUNTRACED);
     		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
    	}
	}
}

/*******************void bringForth()************************/
void bringForth(){
	//deletes the las pid popped
	int pid = backgroundStack.top();
	int status;
	backgroundStack.pop();
	cout<<"["<<pid<<"]"<<endl;
	cout.flush();

	while(wait(&status)!=pid);
}