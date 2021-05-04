#include "./builtin.h"
#include "shell.h"

#define SECTOR_SIZE 512

int main() {
	char line[120];

	enableInterrupts();

	while (1) {
		print("Lexington> \0");
		interrupt(0x21, 1, line, 0, 0);
		runCommand(line);
	}
}

void runCommand(char* line) {
	char* command[3];
	int i;

	parseCommand(command, line);
	print("\n\r\0");
	if (stringEqual(command[0], "type\0")) {
		typeCommand(command[1]);
	} else if (stringEqual(command[0], "execute\0")) {
		executeCommand(command[1]);
	} else if (stringEqual(command[0], "execforeground\0")) {
		executeForeground(command[1]);
	} else if (stringEqual(command[0], "dir\0")) {
		dirCommand();
	} else if (stringEqual(command[0], "copy\0")) {
		copyCommand(command[1], command[2]);
	}	else if (stringEqual(command[0], "kill\0")) {
		killCommand(command[1]);
	} else {
		print("Bad Command!\0");
	}
	print("\n\r\0");
}

void parseCommand(char** command, char* line) {
	int i;
	char cmd[120];
	char arg1[120];
	char arg2[120];

	i = 0;
	i += getTillChar(line, cmd, ' ') + 1;
	i += getTillChar(line+i, arg1, ' ') + 1;
	getTillChar(line+i, arg2, '\0');

	command[0] = cmd;
	command[1] = arg1;
	command[2] = arg2;
}

int getTillChar(char* in, char* out, char c) {
	int i;
	char current;

	i = 0;
	current = in[i];
	while (current != c && current != '\0') {
		out[i] = current;
		current = in[++i];
	}

	out[i] = '\0';

	return i;
}

int stringEqual(char* s1, char* s2) {
	int i = 0;

	while (s1[i] != '\0' && s2[i] != '\0') {
		if (s1[i] != s2[i]) {
			return 0;
		}
		i++;
	}
	return 1;
}

void killCommand(char* segment) {
	
	interrupt(0x21, 9, segment[0] - 48, 0, 0); 
}

void typeCommand(char* filename) {
	char buffer[13312];

	interrupt(0x21, 3, filename, buffer, 0);
	print(buffer);
}

void executeCommand(char* filename) {
		interrupt(0x21, 4, filename, 0x2000, 0);
}

void executeForeground(char* filename) {
		interrupt(0x21, 10, filename, 0x2000, 0);
}

void print(char* line) {
		interrupt(0x21, 0, line, 0, 0);
}

void printNum(int num) {
	char c[2];

	c[1] = '\0';

	while (num > 0) {
		c[0] = (char) mod(num, 10) + 48;
		num /= 10;
		print(c);
	}
}

int mod(int a, int b) {
	int temp = a;
	while (temp >= b) { temp -= b; }
	return temp;
}

void dirCommand() {
	char buffer[SECTOR_SIZE];
	int i;

	interrupt(0x21, 2, buffer, 2, 0);

	for (i = 1; i < 16; i++) {
		if (buffer[i * 32] != 0) {
			buffer[i * 32 + 6] = '\0';
			print(buffer + i * 32);
			print(" \0");
		}
	}
}

void copyCommand(char* file, char* newLocation) {
	char buffer[13312];
	char dir[512];
	int i;
	int j;
	int offset;

	interrupt(0x21, 3, file, buffer, 0);
	interrupt(0x21, 2, dir, 2, 0);

	for (i = 0; i < 16; i++) {
		offset = i * 32;

		for (j = 0; j < 6; j++) {
			if (dir[j + offset] != file[j]) { break; }
		}
		if (
			(j == 6 && file[j+1] == '\0') ||
			(file[j] == '\0' && dir[j + offset] == '\0')
		) {
			j = 6;
			while (dir[offset+j] != 0x00) { j++; }
			break;
		}
	}

	interrupt(0x21, 8, newLocation, buffer,	j - 6);
}
