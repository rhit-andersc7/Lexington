#include "kernel.h"
#include "./builtin.h"

/*
 * Team Members:
 * - Cullen LaKemper
 * - Cooper Anderson
 * - Russel Staples
 */

#define SECTOR_SIZE 512
#define NUM_PROCESSES 8

Process processes[NUM_PROCESSES];
int currentProcess;

int main() {
	int i;

	for (i = 0; i < NUM_PROCESSES; i++) {
		processes[i].active = 0;
		processes[i].pointer = 0xff00;
		processes[i].sector = (i + 2) * 0x1000;
	}

	currentProcess = -1;
	makeInterrupt21();
	interrupt(0x21, 4, "shell\0", 0x2000, 0);
	makeTimerInterrupt();

	while(1) {}

	return 0;
}

void terminate() {
	setKernelDataSegment();
	processes[currentProcess].active = 0;
	while(1) {}
}

void executeProgram(char* name) {
	char error[2];
	char buffer[13312];
	int i;
	int fileSize;
	int segment;
	int s;

	error[0] = 'X';
	error[1] = '\0';

	fileSize = readFile(name, buffer);

	if (fileSize == -1) {
		printString(error);
		return;
	}

	setKernelDataSegment();
	for (s = 0; s < NUM_PROCESSES; s++) {
		if (processes[s].active == 0) {
			break;
		}
	}

	if (s == NUM_PROCESSES) {
		error[0] = 'Y';
		printString(error);
		return;
	}

	processes[s].active = 1;
	processes[s].pointer = 0xff00;
	/* segment = processes[s].sector; */
	segment = (s + 2) * 0x1000;
	currentProcess = s;
	restoreDataSegment();

	for (i = 0; i < fileSize; i++) {
		putInMemory(segment, i, buffer[i]);
	}

	initializeProgram(segment);
}

int readFile(char* file, char* buffer) {
	char notFoundMessage[2];
	char directory[SECTOR_SIZE];
	int i;
	int offset;
	int j;

	notFoundMessage[0] = 'N';
	notFoundMessage[1] = '\0';

	readSector(directory, 2);

	for (i = 0; i < 16; i++) {
		offset = i*32;
		for (j = 0; j < 6; j++) {
			if (directory[j+offset] != file[j]) {
				break;
			}
		}
		if (
			(j == 6 && file[j+1] == '\0') ||
			(file[j] == '\0' && directory[j+offset] == '\0')
		) {
			j = 6;
			while(j < 32 && directory[offset+j] != 0) {
				readSector(buffer+(j-6)*512, directory[offset+j]);
				j++;
			}

			return (j-5)*512;
		}
	}
	printString(notFoundMessage);
	buffer[0] = '\0';
	return -1;
}

void printString(char* text) {
	char* chars = text;
	while (*chars != '\0') {
		interrupt(0x10, 0xe * 256 + *chars++, 0, 0, 0);
	}
}

void printCharLocation(char c, int row, int column) {
	int memAddress = 0x8000 + row*160 + column*2;

	putInMemory(0xB000, memAddress, c);
	putInMemory(0xB000, memAddress + 1, 0x7);
}

void printStringLocation(char* text, int row, int column) {
	int i = 0;
	char c = text[i];

	while (c != '\0') {
		printCharLocation(c, row, column);
		c = text[++i];

		column++;
		if (column >= 80) {
			row++;
			column = 0;
		}
	}
}

void readString(char* line) {
	int i = 0, lineLength = 80, ax = 0;
	char charRead, backSpace = 0x8, enter = 0xd;
	charRead = interrupt(0x16, ax, 0, 0, 0);
	while (charRead != enter && i < lineLength-2) {
		if (charRead != backSpace) {
			interrupt(0x10, 0xe*256+charRead, 0, 0, 0);
			line[i++] = charRead;
		} else {
			i--;
			if (i >= 0) {
				interrupt(0x10, 0xe*256+charRead, 0, 0, 0);
				interrupt(0x10, 0xe*256+'\0', 0, 0, 0);
				interrupt(0x10, 0xe*256+backSpace, 0, 0, 0);
			} else {
				i = 0;
			}
		}
		charRead = interrupt(0x16, ax, 0, 0, 0);
	}
	line[i] = 0xa;
	line[i+1] = 0x0;

	/* correctly prints a newline */
	printString("\r\n");

	return;
}

int mod(int a, int b) {
	int temp = a;
	while (temp >= b) {
		temp -= b;
	}
	return temp;
}

int div(int a, int b) {
	int quotient = 0;
	while ((quotient + 1) * b <= a) {
		quotient++;
	}
	return quotient;
}

void readSector(char* buffer, int sector) {
	int relativeSector = mod(sector, 18) + 1;
	int head = mod(div(sector, 18), 2);
	int track = div(sector, 36);
	interrupt(0x13, 513, buffer, track*256+relativeSector, head*256);
}

void writeSector(char* buffer, int sector) {
	int relativeSector = mod(sector, 18) + 1;
	int head = mod(div(sector, 18), 2);
	int track = div(sector, 36);
	interrupt(0x13, (3 << 8) + 1, buffer, track*256+relativeSector, head*256);
}

void writeFile(char* name, char* contents, int sectors) {
	char error[2];
	char map[SECTOR_SIZE];
	char dir[SECTOR_SIZE];
	int i;
	int j;
	int k;
	int offset;

	error[0] = 'F';
	error[1] = '\0';

	readSector(map, 1);
	readSector(dir, 2);

	for (i = 1; i < 16; i++) {
		offset = i * 32;
		if (dir[offset] == 0x00) {
			for (j = 0; j < 6; j++) {
				dir[offset+j] = name[j];
				if (name[j] == '\0') {
					break;
				}
			}

			for (j = j; j < 6; j++) {
				dir[offset+j] = '\0';
			}
			i = 0;
			for (k = 0; k < SECTOR_SIZE; k++) {
				if (map[k] == 0x00) {
					map[k] = 0xFF;
					dir[offset+j] = k;
					j++;
					writeSector(contents+i*SECTOR_SIZE, k);
					if (++i == sectors) {
						break;
					}
				}
			}

			if (k == SECTOR_SIZE) {
				return;
			}

			for (j = j; j < 32; j++) {
				dir[j] = 0x00;
			}

			writeSector(dir, 2);
			writeSector(map, 1);

			return;
		}
	}
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
	switch(ax) {
		case 0:
			printString(bx);
			break;
		case 1:
			readString(bx);
			break;
		case 2:
			readSector(bx, cx);
			break;
		case 3:
			readFile(bx, cx);
			break;
		case 4:
			executeProgram(bx);
			break;
		case 5:
			terminate();
			break;
		case 6:
			writeSector(bx, cx);
			break;
		case 8:
			writeFile(bx, cx, dx);
			break;
		default:
			printString("Error Not a Function!\0");
	}
}

void handleTimerInterrupt(int segment, int sp) {
	int active;

	if (currentProcess != -1) {
		processes[currentProcess].pointer = sp;
	}

	while (1) {
		currentProcess = mod(currentProcess + 1, NUM_PROCESSES);
		active = processes[currentProcess].active;
		if (active == 1) { break; }
		else if (active == 2) {
		}
	}

	returnFromTimer(
		(currentProcess + 2) * 0x1000,
		processes[currentProcess].pointer
	);
}
