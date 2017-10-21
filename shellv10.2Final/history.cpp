#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
using namespace std;
#define handle_error(msg) \
	do{ perror(msg); exit(EXIT_FAILURE);}while(0);


#include "shell.h"
int LIMIT=0;
int EXITING;
void execution(string);
void execute();
void split_commands();
int parse(string);

vector<string> shell_history;

void loadHistory(){
	EXITING=1;
	string line;
	shell_history.clear();

	ifstream fd("./HistoryFile");
	while(getline(fd,line)){
		shell_history.push_back(line);
	}
}

void storeHistory(){
	int fd1 = open("./HistoryFile",O_CREAT | O_RDWR ,0777);
	int i=0;
	int size = shell_history.size();
	string newline = "\n";
	for(i=0;i<size;i++){
		write(fd1 , shell_history[i].c_str() , shell_history[i].length());
		write(fd1 , newline.c_str() , newline.length());	
	}
}

void history(string input){
	if(shell_history.size() == 0){
		if(input[0]!=' ')shell_history.push_back(input);
		return;
	}
	if(shell_history[shell_history.size()-1].compare(input)!=0 && input[0]!=' ' && input[0]!='!' ) shell_history.push_back(input);
}

void viewHistory(int n){
	int option = command_options[n];
	int i=0;
	int size = shell_history.size();
	if(option >1) LIMIT = stoi(command_parts[n][1],nullptr,10); 
	else LIMIT = size;
	i=size-LIMIT;
	int count=0;
	if(i<0) i=0;
	for(;i<size;i++){
			cout<<count+1<<"  "<<shell_history[i]<<endl;
			count++;
	}
}

void bang(int commandNumber){
	string bang_command = commands[commandNumber];
	string to_exec = "";
	int length = bang_command.length();
	if(length==1) {
		handle_error("Invalid ! command");

	}
	string option = bang_command.substr(1,length-2);

	if(option.compare("!")==0 || option.compare("-1")==0){
		to_exec = shell_history[shell_history.size()-1];
		cout<<to_exec<<endl;
		execution(to_exec);
		
	}else if(option.at(0)=='-'){
		int loc = stoi(option,nullptr,10);
		to_exec = shell_history[shell_history.size()+loc];
		cout<<to_exec<<endl;
		execution(to_exec);

	}else if(option.at(0)>='0' && option.at(0)<='9'){
		int loc = stoi(option,nullptr,10);
		to_exec = shell_history[loc-1];
		cout<<to_exec<<endl;
		execution(to_exec);
	}else if(option.at(0)==' '){
		to_exec = option.substr(1,option.length()-2);
		cout<<to_exec<<endl;
		execution(to_exec);
	}else{
		unsigned int i=shell_history.size()-1;
		for(;i>=0;i--){
			string k1,k2;
			//bang string
			int loc = option.find(" ");
			if(loc >0) k1 = option.substr(0,loc);
			else k1 = option;

			k2 = shell_history[i].substr(0,k1.length());

			if(k1.compare(k2)==0) {
					break;
			}
		}
		if(i==shell_history.size()){
			handle_error("command not found");
		}else{
			to_exec = shell_history[i];
		}
		cout<<to_exec<<endl;
		execution(to_exec);

	}
}


void execution(string input){
	if(input.compare("exit")==0) EXITING=0;
	else{
		number_of_commands = parse(input);
		split_commands();
		int i=0;
		for(i=0;i<number_of_commands;++i){
				removeQuotes(i);
		}
		if(number_of_commands==1){
			if(command_parts[0][0].compare("cd")==0){
				if(changeDirectory(0)<0){
					cout<<"Directory not found"<<endl;
				}
			}else if(command_parts[0][0].compare("export")==0){
				exportimpl(0);
			}else{
				execute();
			}	
		}
	}
}