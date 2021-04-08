#include "kernel.h"
#include "./builtin.h"

/*
 * Team Members:
 * - Cullen LaKemper
 * - Cooper Anderson
 * - Russel Staples
 */

int main() {

  char buffer[13312];
  makeInterrupt21();

  // 0 is print, 1 is read, 2 is readSector
  interrupt(0x21, 3, "messag", buffer);










  //char line[80];
  // interrupt(0x21,0,"Enter line:\0",0,0);
 // interrupt(0x21,1,line,0,0);
 // interrupt(0x21,0,line,0,0);
 // interrupt(0x21,2,buffer,30);
 // interrupt(0x21,0,buffer,0);
  while(1); /* never forget this */
	return 0;
}

void readFile(char* file, char* buffer){

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
		default:
			printString("Error Not a Function!\0");
	}
}
