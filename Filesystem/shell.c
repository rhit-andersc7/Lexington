#include "shell.h"

int main(){

	char line[120];

	while(1){
		interrupt(0x21, 0, "Lexington> \0", 0, 0);
	}
}

void runCommand(char* line){
	char* command[3];
	int i;
	
	parseCommand(command, line);
	
	if(stringEqual(command[0],"type\0")){
		typeCommand(command[0]);
	}
	else if(stringEqual(command[0],"execute\0")){
		executeCommand(command[1]);
	}
	else if(stringEqual(command[0],"dir\0")){
		dirCommand();
	}
	else if(stringEqual(command[0],"copy\0")){
		copyCommand(command[1], command[2]);
	}
	else{
		interrupt(0x21, "Bad Command!", error, 0, 0);
	}
}

void parseCommand(char** command, char* line){
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
			command[spaces][i] = c;
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
void typeCommand(char* filename){
        interrupt(0x21, 3, filename, 0x2000, 0);
}

void executeCommand(char* filename){
        interrupt(0x21, 4, filename, 0x2000, 0);
}

void dirCommand(){
/*
	int i;
	int j;
	int offset;
	char directory[];

		for(i = 0; i < 16; i++){
                offset = i*32;
                for(j = 0; j < 6; j++){
        		interrupt(0x21, 3, directory[j+offset], 0x2000, 0);
                }
        }
*/
}

void copyCommand(char* file, char* newLocation){

}
