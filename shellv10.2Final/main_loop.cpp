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

char blue[] = { 0x1b, '[', '1', ';', '3', '5', 'm', 0};
char mag[] = { 0x1b, '[', '1', ';', '3', '5', 'm', 0};
char plain[] = { 0x1b, '[', '0', ';', '3', '9', 'm', 0};

void split_commands();
int parse(string);
int changeDirectory();
int getCWD();
int history();
void viewHistory(int);
void loadHistory();
void storeHistory();
void execute();
void run(int);
int echo(int);
void exportimpl(int);

void handler(int){
	cout<<endl<<mag<<"My_Shell~";
	cout<<blue;
	int status = getCWD();
	if(status==-1){
		handle_error("current working directory name is too long");
	}
	cout<<">>"<<plain;
	cout.flush();
}

int main(){
    loadHistory();
    signal(SIGINT, handler);
	while(EXITING){
		//prompt display

		cout<<mag<<"My_Shell~";
		cout<<blue;	
		int status = getCWD();
		if(status==-1){
			handle_error("current working directory name is too long");
		}
		cout<<">>"<<plain;

	//parsing the input
		string input ;

		getline(cin,input);
		if(input.length()==0) continue;
		if(input.compare("exit")==0){
			history(input);
			cout<<"Bye..."<<endl;
			storeHistory();
			exit(0);
		}
				
		input = input + " ";
		//checking if input is only a space
		int i;
		int len = input.size();
		for(i=0;i<len;i++){
			if(input[i]!=' ') break;
		}
		if(i==len) continue;
		/*******************Calling functions********************/
		history(input);
		number_of_commands = parse(input);
		split_commands();	

		
		for( i = 0; i < number_of_commands; ++i)
		{
			removeQuotes(i);
		}


		if(number_of_commands==1){
			if(command_parts[0][0].compare("cd")==0){
				if(changeDirectory(0)<0){
					cout<<"Directory not found"<<endl;
				}
			}else if(command_parts[0][0].compare("export")==0){
				exportimpl(0);
			}else if(command_parts[0][0].at(0)=='!'){
				bang(0);
			}else if(command_parts[0][0].compare("fg")==0){
				bringForth();
			}else{
				execute();
			}	
		}else{
			execute();
		}
	}
	return 0;
}