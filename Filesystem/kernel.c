#include "kernel.h"
#include "./builtin.h"

/*
 * Team Members:
 * - Cullen LaKemper
 * - Cooper Anderson
 * - Russel Staples
 */

#define SECTOR_SIZE 512

int main() {

	char buffer[13312];
	makeInterrupt21();

	interrupt(0x21, 3, "messag\0", buffer, 0);
	interrupt(0x21, 0, buffer, 0, 0);

	while(1); /* never forget this */
	return 0;
}

void executeProgram(char* name, int segment){
	char error[2];
	char buffer[13312];
	int i;
	int fileSize;

	error[0] = 'X';
	error[1] = '\0';

	fileSize = readFile(name, buffer);

	if(fileSize == -1){
		printString(error);
		return;
	}

	for(i=0; i<fileSize; i++){
		putInMemory(segment, i, buffer[i]);
	}

	launchProgram(segment);
}

int readFile(char* file, char* buffer){	
	char notFoundMessage[2];
	char directory[SECTOR_SIZE];
	int i;
	int offset;
	int j;

	notFoundMessage[0] = 'N';
	notFoundMessage[1] = '\0';

	readSector(directory, 2);

	for(i = 0; i < 512; i+=32){
		printString(directory[i]);
	}

	for(i = 0; i < 16; i++){
		offset = i*32;
		for(j = 0; j < 6; j++){
			if(directory[j+offset]!=file[j]){
				break;
			}
		}
		if(j==6 || (file[j]=='\0' && directory[j+offset]=='\0')){
			
			j=6;

			while(j<32 && directory[offset+j]!=0){
				readSector(buffer+(j-6)*512, directory[offset+j]);
			}
				
			return j-6+1;
		}
	}
	printString(notFoundMessage);
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
		if(column >= 80) {
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
			executeProgram(bx,cx);
			break;
		default:
			printString("Error Not a Function!\0");
	}
}
