#include<iostream>
#include<string>
#include<string.h>
#include<stdio.h>
#include<map>

using namespace std;
#define handle_error(msg) \
	do{ perror(msg); exit(EXIT_FAILURE);}while(0);

#include "shell.h"

int printVal(string);

int dollar[100][20];

void removeQuotes(int n){
	int arguments = command_options[n]; //echo is one 
	int i;
    int quote_loc[100];
 
    dollar[n][0]=-2;
	for(i=1;i<arguments;i++){
		//parse eache string
		dollar[n][i]=-1;
		int s_q=0,d_q=0;	
		string temp = command_parts[n][i];
		int len = temp.length();
		int j=0,k=0;
		for(j=0;j<len;j++){
			if(temp.at(j)=='\'' && s_q==0 && d_q==0){
					s_q++;
					quote_loc[k++]=j;
			}
			else if(temp.at(j)=='\'' && s_q==1 && d_q==0){
				s_q--;
				quote_loc[k++]=j;
			}
			else if(temp.at(j)=='"' && s_q==0 && d_q==0){
					d_q++;
					quote_loc[k++]=j;
			}
			else if(temp.at(j)=='"' && s_q==0 && d_q==1){
				d_q--;
				quote_loc[k++]=j;
			}
		}
		if(s_q!=0 || d_q!=0){
			handle_error("invalid input in echo");
		}

		for(j=0;j<k;j++){
			temp.erase(quote_loc[j]-j,1);
		}

		for(j=0;j<temp.length();j++){
			if(temp.at(j)=='$' && dollar[n][i]==-1) dollar[n][i]=j;
		}

		command_parts[n][i] = temp;
	}
}

int echo(int n){
	int arguments = command_options[n]; //echo is one 
	int i;
	loadMap();
	for(i=1;i<arguments;i++){
		string c = command_parts[n][i];
		if(dollar[n][i]>=0){
			int len = c.length()-1;
			string var = c.substr(0,dollar[n][i]);
			cout<<var;
			var = c.substr(dollar[n][i]+1,len);
			//cout<<var;
			char * path = getenv(var.c_str());
			if(printVal(var) == -1){
				if(path!=NULL) printf("%s ",path);
			}	
		}else{
			cout<<c<<" ";
		}
	}
	cout<<endl;
	return 0;
}
