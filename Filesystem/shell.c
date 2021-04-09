#include "shell.h"

int main(){

	char prompt[5];
	char line[120];

	prompt = "Lexington> ";

	while(1){
		interrupt(0x21, 0, prompt, 0, 0);
		interrupt(0x21, 1, line, 0, 0);
		runCommand(line); 
	}

}

void runCommand(char* line){
	char** parsedCommand[3];
	char error[3];
	int i;
	
	error = "Bad Command!"
	
	parseCommand(parsedCommand, line);
	
	if(stringEqual(parseCommand[0],"type\0")){
		type(arg1);
	}
	else if(stringEqual(parseCommand[0],"execute\0")){
		execute(arg1);
	}
	else if(stringEqual(parseCommand[0],"dir\0")){
		dir();
	}
	else if(stringEqual(parseCommand[0],"copy\0")){
		copy(arg1, arg2);
	}
	else{
		interrupt(0x21, 0, error, 0, 0);
	}
}

void parseCommand(char** parsedCommand, char* line){
	int i;
	int spaces;
	char c;

	i = 0;
	spaces = 0;
	c = line[i];

	while(c != '\0'){
		if(c==' '){
			spaces++;
		}
		else{
			parsedCommand[spaces][i] = c;
		}
		c = line[++i];
	}
}

int stringEqual(char* s1, char* s2){
	int i = 0;

	while(s1[i]!='\0'&&s2[i]!='\0'){
		if(s1[i]!=s2[i]){
			return 0;
		}
		i++;
	}
	return 1;
}

