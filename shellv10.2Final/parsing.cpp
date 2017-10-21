#include<iostream>
#include<string>
#include<string.h>
#include<stdio.h>
#define handle_error(msg) \
	do{ perror(msg); exit(EXIT_FAILURE);}while(0);

#define ten5 100000

#define PIPE 1
#define IREDIRECTION 2
#define OREDIRECTION 3
#define BG 4

using namespace std;

#include "shell.h"

int mode1, mode2, mode3;
string commands[100],inputFile,outputFile;
string command_parts[100][10000];	
int pipe_loc[1000];
int number_of_commands;
int command_options[100];

int parse(string input){
	
	int len = input.length();
	int i=0,k=0;
	int s_quote=0,d_quote=0; //1 means quote encountered
	
	i=0;
	k=0;
	while(i<len){
		if(input[i]=='\'' && s_quote==0){
				s_quote++;
		}
		else if(input[i]=='\'' && s_quote==1){
				s_quote--;
		}
		else if(input[i]=='"' && d_quote==0){
				d_quote++;
			}
		else if(input[i]=='"' && d_quote==1){
				d_quote--;
		}
		else if(s_quote==0 && d_quote==0 && input[i]=='|'){
				pipe_loc[k++]=i;
		}
		i++;
	}
	//now divide the string on basis of the pipe_locations found
	//number of commands is k+1
	
	int start=0, substr_length = pipe_loc[0]-1;
	for(i=0;i<k+1;i++){
		commands[i] = input.substr(start,substr_length);
		start = pipe_loc[i] +1;
		substr_length = pipe_loc[i+1] - pipe_loc[i] -1;
	}

    return (k+1);
}


string removeTrailingSpace(string s){
	int len = s.length();
	int i=0;
	while(i<len && (s[i]!=' ' || s[i]!='\t' || s[i]!='\n')){
		i++;
	}
	if (i==0) return "";
	s=s.substr(0,i);
	return s;
}


void identifyFile(){
	int j=0;

	for (int i = 0; i < number_of_commands; ++i)
	{
		for(j=0;j<command_options[i];j++){
			if(command_parts[i][j].compare("<")==0){
				inputFile = command_parts[i][j+1];
			}
			if(command_parts[i][j].compare(">")==0 ) {
				outputFile = command_parts[i][j+1];
			}
		}
	}

}

void split_commands(){
	int i,n,j=0,k=0;
	mode1=0;mode2=0;mode3=0;
	for(n=0;n<number_of_commands;n++){
		string temp = commands[n];
		temp=temp+" "; //assign command[j] to a string for comparision
		int len = temp.length();

		//initiallise variables and store space location of each sub command
		i=0,k=0;
		int s_quote=0,d_quote=0; 
		int space_loc[100];

		//removing initial space
		while(i<len){
			if(temp[i]==' ' || temp[i]=='\t') i++;
			else break;
		}

		//first character other than space or tab encountered
		int start=i;

		while(i<len){
			if(temp[i]=='\'' && s_quote==0){
					s_quote++;
			}
			else if(temp[i]=='\'' && s_quote==1){
					s_quote--;
			}
			else if(temp[i]=='"' && d_quote==0){
							d_quote++;
			}
			else if(temp[i]=='"' && d_quote==1){
					d_quote--;
			}
			else if(s_quote==0 && d_quote==0){
				if(temp[i]==' '){
					space_loc[k++]=i;
				}else if(temp[i]=='<'){
					mode1 = 1;
					//inputFile = temp[i+1];
				}else if(temp[i]=='>'){
					mode2 = 1;
					//outputFile = temp[i+1];
				}else if(temp[i]=='&'){
					mode3 = 1;
				}
			}
			i++;
		}

		j=0;
		int leng=space_loc[j]-start;

		for(i=0;i<k+1 && j<k;i++){
			command_parts[n][i]=temp.substr(start,leng);
			start=space_loc[j]+1;
			j++;
			while(j<k && space_loc[j]==start ){
				start++;j++;
			}
			leng=space_loc[j]-start;
		}
		
		//when at the end of command there is no space then handle it

		if(space_loc[j]+1<len){ 
			command_parts[n][i]=temp.substr(start,(len-start));
			command_parts[n][i]=removeTrailingSpace(command_parts[n][i]);
			if(leng>0){
				i++;
			}
		}
		command_parts[n][i]="";
		command_options[n] = i;
	}
	identifyFile();

	for(n=0;n<number_of_commands;n++){
		for(j=0;j<command_options[n];j++){
			if(command_parts[n][j].compare("<")==0 ||command_parts[n][j].compare(">")==0){
				command_options[n]=j;
				break;
			}
		}
	}
}
