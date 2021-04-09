#include "shell.h"

int main(){

	char prompt[5];
	char line[120];

	prompt[0]='L';
	prompt[1]='>';
	prompt[2]=' ';
	prompt[3]=' ';
	prompt[4]='\0';

	while(1){
		interrupt(0x21, 0, prompt, 0, 0);
		interrupt(0x21, 1, line, 0, 0);
		runCommand(line); 
	}

}

void runCommand(char* line){


}
