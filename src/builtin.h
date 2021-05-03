int interrupt(int number, int ax, char* bx, int cx, int dx);
void makeInterrupt21();
void makeTimerInterrupt();
void putInMemory(int segment, int address, char character);
void enableInterrupts();
void launchProgram(int segment);
void initializeProgram(int segment);
void returnFromTimer(int segment, int sp);
