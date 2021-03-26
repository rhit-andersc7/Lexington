void printString(char*);
void printCharLocation(char, int, int);
void printStringLocation(char*, int, int);
void readString(char*);
int mod(int, int);
int div(int, int);
void readSector(char*, int);
handleInterrupt21(int, int, int, int);
  
int main() {

  char buffer[512];
	char line[80];
	printStringLocation("Hello World With Location", 0, 0);
	             
	printString("Enter a line: \0");
	readString(line);
	printString(line);

  readSector(buffer, 30);
  printString(buffer);

  makeInterrupt21(0x21,0,0,0,0);

	while(1); /* never forget this */
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
	int memAddress = 0x8000 + row*160 + column*2;

	putInMemory(0xB000,memAddress,c);
	putInMemory(0xB000,memAddress+1, 0x7);
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

void readString(char *line){
  int i, lineLength, ax;
  char charRead, backSpace, enter;
  lineLength = 80;
  i = 0;
  ax = 0;
  backSpace = 0x8;
  enter = 0xd;
  charRead = interrupt(0x16, ax, 0, 0, 0);
  while (charRead != enter && i < lineLength-2) {
    if (charRead != backSpace) {
      interrupt(0x10, 0xe*256+charRead, 0, 0, 0);
      line[i] = charRead;
      i++;
    } else {
      i--;
      if (i >= 0) {
	interrupt(0x10, 0xe*256+charRead, 0, 0, 0);
	interrupt(0x10, 0xe*256+'\0', 0, 0, 0);
	interrupt(0x10, 0xe*256+backSpace, 0, 0, 0);
      }
      else {
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

int mod(int a, int b){
  int temp;
  temp = a;
  while (temp >= b) {
    temp = temp-b;
  }
  return temp;
}

int div(int a, int b){
  int quotient;
  quotient = 0;
  while ((quotient + 1) * b <= a) {
    quotient++;
  }
  return quotient;
}

void readSector(char *buffer, int sector){
    int relativeSector = mod(sector, 18) + 1;
    int head = mod(div(sector,18),2);
    int track = div(sector, 36);
		interrupt(0x13, 513, buffer, track*256+relativeSector, head*256);
}

void handleInterrupt21(int ax, int bx, int cx, int dx){
  printString("Hello World\0");
}
