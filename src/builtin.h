int interrupt(int number, int ax, char* bx, int cx, int dx);
void makeInterrupt21();
void makeTimerInterrupt();
void putInMemory(int segment, int address, char character);
void enableInterrupts();