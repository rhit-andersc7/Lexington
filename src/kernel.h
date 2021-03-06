void printString(char* text);
void printCharLocation(char c, int row, int column);
void printStringLocation(char* text, int row, int column);
void readString(char* line);
int mod(int a, int b);
int div(int a, int b);
void readSector(char* buffer, int sector);
void handleInterrupt21(int ax, int bx, int cx, int dx);
void handleTimerInterrupt(int segment, int sp);
int readFile(char*, char*);
int executeProgram(char*);
void terminate();
void writeSector(char*, int);
void writeFile(char*, char*, int);
void getSectors(char*, int*);
void wait(char* program);

struct process {
	int active;
	int pointer;
	int wait;
};
typedef struct process Process;
