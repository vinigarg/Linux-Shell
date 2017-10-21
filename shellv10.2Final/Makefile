# TARGET = name of executable you want to create 
TARGET=sample
# If you are using gcc to compile then make CC=gcc
CC=g++
# list of headerfiles space seperated
HEADERS=shell.h
#flags  
FLAGS = -std=c++11
# you can add flags like this: FLAGS += -std=c++0x

#list of object files that you want to create[space seperated].
OBJS=parsing.o cd_pwd.o echo.o export.o execvpimpl.o execution.o history.o main_loop.o 

$(TARGET) : $(OBJS) $(HEADERS)
	$(CC) $(FLAGS) -o $@ $^
	
%.o : %.cpp 
	$(CC) $(FLAGS) -c -o $@ $^

# clean the build, removes all .o files and executable named in TARGET. To execute run make clean from command line
clean : 
	rm *.o $(TARGET)
