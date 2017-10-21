#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <map>
#define handle_error(msg) \
	do{ perror(msg);}while(0);

using namespace std;

#include "shell.h"

map <string, string> local_variables;

void exportimpl(int n){
	//write in file
	int fd = open("./export.txt" , O_CREAT | O_APPEND | O_RDWR ,0777);
	int parts = command_options[n],i=0;
	for(i=1;i<parts;i++){
		string cmd = command_parts[n][i];
		cmd += '\n';
		write(fd,cmd.c_str(),cmd.length());
	}
}


int checkForLocalAssignment(int n,int type){
	int parts = command_options[n];
	if(parts==1 || parts==2){
		//check for presence of assignment operator ...not within quotes
		string command = command_parts[n][parts-1];
		int length = command.length();
		int i=0;
		int equal=-1;
		for(i=0;i<length && equal==-1;i++){
			if(command[i]=='='){
					equal=i;
			}
		}
		if(equal>0){
			string var,val;
			var = command.substr(0,equal);
			val = command.substr(equal+1,length - equal);
			local_variables[var] = val;
			return 0;
		}
	}
	return -1;
}


void loadMap(){
	local_variables.clear();
	int fd = open("./export.txt" , O_CREAT | O_APPEND | O_RDWR ,0777);
	char buffer[10000];
	int rd,j=0;
	rd = read(fd, &buffer , sizeof(buffer));
	int i=0 , equal=-1;
	string s=""; 
	while(buffer[i]!= '\0'){ 
		s="";
		while(buffer[i]!='\n' && buffer[i]!= '\0'){
			s+= buffer[i];
			i++;
		}
		if(s!="") {
			equal=-1;
			int length = s.length();
			for(j=0;j<length && equal==-1;j++){
			if(s[j]=='='){
					equal=j;
				}
			}
			if(equal>0){
				string var,val;
				var = s.substr(0,equal);
				val = s.substr(equal+1,length - equal);
				local_variables[var] = val;
			}
		}
		//cout<<"k";	
		i++;
	}	
}


int printVal(string key){
	if(local_variables.find(key)!=local_variables.end())  cout<<local_variables[key]<<" ";
	else return -1;
	return 0;
}