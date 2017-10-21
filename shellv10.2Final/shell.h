#include <map>
#include <iostream>
#include <string>
#include <stack>

using namespace std;

extern stack <int> backgroundStack;
extern int command_options[100];
extern string commands[100];
extern string command_parts[100][10000];	
extern int pipe_loc[1000];
extern int mode1, mode2, mode3;
extern string inputFile;
extern string outputFile;
extern int number_of_commands;
extern int pipes[2];
extern map <string, string> local_variables;
extern int EXITING;

/*****************BF and FG***************/
void bringForth();

/********************MainLoop.cpp*********/
void execute();

/*********************CD.cpp**************/
int changeDirectory(int);
int getCWD();

/************PARSING.cpp****************/
void split_commands();
int parse(string);
string removeTrailingSpace(string);
string removeInitialSpace(string);

/**************ECHO FUNCTIONS***************/
int echo(int);
void removeQuotes(int);

/************HISTORY.cpp*********************/
void history(string);
void viewHistory(int);
void loadHistory();
void storeHistory();
void createHistory();
void bang(int); // argument is the number of command

/*************EXECVP.cpp*********************/
void run(int);

/************EXPORT.cpp**********************/
int checkForLocalAssignment(int,int);
int printVal(string);
void exportimpl(int);
void loadMap();
