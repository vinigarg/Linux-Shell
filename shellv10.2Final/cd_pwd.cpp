#include<unistd.h>
#include<iostream>
#include<string>
#include<limits.h>

using namespace std;

#include "shell.h"

string OLDPWD = "";
static int cd_called=0;

int changeDirectory(int n){

/* to handle OLDPWD seperate from getCWD */
	char * cwd ;
	char buffer[PATH_MAX +1];
	cwd = getcwd(buffer , PATH_MAX +1);	

/* CD syntax handling */
	int number_of_paths = command_options[n];
	string path[number_of_paths];
	
	int status,i;

	for(i=0;i<number_of_paths;i++){
		path[i] = command_parts[n][i]; //path	
	}

	if(number_of_paths==0 || path[1].compare("--")==0 || path[1].compare("~")==0 || path[1].at(0)=='#' || path[1].compare("~/")==0 ){
		cd_called++;
		string home = "HOME";
		status = chdir(getenv(home.c_str()));
	}else if(path[1]=="-"){
			if(OLDPWD == "") {
				cout<<"OLDPWD not set"<<endl;
			}else{
				cout<<OLDPWD<<endl;
				status = chdir(OLDPWD.c_str());	
			}
	}else{
		cd_called++;
		status = chdir(path[1].c_str());	
	} 

	if(cd_called > 0){
		OLDPWD = cwd;
	}

	return status;
}

int getCWD(){
	char * cwd ;
	char buffer[PATH_MAX +1];
	cwd = getcwd(buffer , PATH_MAX +1);
	if(string(cwd).length() > 0) cout<<cwd;
	else return -1;
	return 0;
}


