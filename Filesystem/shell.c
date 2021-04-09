#include "shell.h"

int main(){

	char line[120];

	while(1){
		print("Lexington> \0");
		interrupt(0x21, 1, line, 0, 0);
		runCommand(line);
	}
}

void runCommand(char* line){
	char* command[3];
	int i;
	
	parseCommand(command, line);
	print("\n\r\0");
	if(stringEqual(command[0],"type\0")){
		typeCommand(command[1]);
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
		print("Bad Command!\0");
	}
	print("\n\r\0");
}

void parseCommand(char** command, char* line){
	int i;
	char cmd[120];
	char arg1[120];
	char arg2[120];

	i = 0;
	i += getTillChar(line, cmd, ' ')+1;
	i += getTillChar(line+i, arg1, ' ')+1;	
	getTillChar(line+i, arg2, '\0');

	command[0] = cmd;
	command[1] = arg1;
	command[2] = arg2;
}

int getTillChar(char* in, char* out, char c){
	int i;
	char current;

	i=0;
	current = in[i];
	while(current!=c && current!='\0'){
		out[i] = current;
		current = in[++i];
	}

	out[i] = '\0';

	return i;
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
	char buffer[13312];

	interrupt(0x21, 3, filename, buffer, 0);
	print(buffer);
}

void executeCommand(char* filename){
    interrupt(0x21, 4, filename, 0x2000, 0);
}

void print(char* line){
    interrupt(0x21, 0, line, 0, 0);
}

void printNum(int num){
	char c[2];

	c[1]='\0';

	while(num > 0 ){
		c[0] = (char) mod(num,10) + 48;
		num/=10;
		print(c);
	}

}

int mod(int a, int b) {
	int temp = a;
	while (temp >= b) {
		temp -= b;
	}
	return temp;
}

void dirCommand(){

}

void copyCommand(char* file, char* newLocation){

}
