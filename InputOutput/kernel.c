void printString(char *);
void printCharLocation(char, int, int);
void printStringLocation(char*, int, int);
int main() {
	printStringLocation("Hello World With Location", 0, 0);
	printString("Hello World");
	while(1) {
	}
	return 0;
}

void printString(char *chars) {
	int i = 0;
	char c = chars[0];
	while(c != '\0') {
		interrupt(0x10, 0xe*256+c, 0, 0, 0);
		i++;
		c = chars[i];
	}
}

void printCharLocation(char c, int row, int column){
	int memRow = 0xB000 + row*160;
	int memCol = memRow + column*2; 

	putInMemory(memRow,memCol,c);
	putInMemory(memRow,memCol+1, 0x7);
}

void printStringLocation(char* string, int row, int column){

	int i = 0;
	char c = string[i];

	while(c!='\0'){
		printCharLocation(c,row,column);
		
		i++;
		column++;
		if(column>=80){
			row++;
			column=0;
		}
		c = string[i];
	}

}
