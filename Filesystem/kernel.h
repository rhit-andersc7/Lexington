void printString(char* text);
void printCharLocation(char c, int row, int column);
void printStringLocation(char* text, int row, int column);
void readString(char* line);
int mod(int a, int b);
int div(int a, int b);
void readSector(char* buffer, int sector);
void handleInterrupt21(int ax, int bx, int cx, int dx);
int readFile(char*, char*);
void executeProgram(char*, int);
void terminate();
void writeSector(char*, int);
void writeFile(char*, char*, int);
