printString(char *);
int main() {

	putInMemory(0xB000, 0x8500, 'H');
	putInMemory(0xB000, 0x8501, 0x7);
	putInMemory(0xB000, 0x8502, 'e');
	putInMemory(0xB000, 0x8503, 0x7);
	putInMemory(0xB000, 0x8504, 'l');
	putInMemory(0xB000, 0x8505, 0x7);
	putInMemory(0xB000, 0x8506, 'l');
	putInMemory(0xB000, 0x8507, 0x7);
	putInMemory(0xB000, 0x8508, 'o');
	putInMemory(0xB000, 0x8509, 0x7);
	putInMemory(0xB000, 0x850A, ' ');
	putInMemory(0xB000, 0x850B, 0x7);
	putInMemory(0xB000, 0x850C, 'W');
	putInMemory(0xB000, 0x850D, 0x7);
	putInMemory(0xB000, 0x850E, 'o');
	putInMemory(0xB000, 0x850F, 0x7);
	putInMemory(0xB000, 0x8510, 'r');
	putInMemory(0xB000, 0x8511, 0x7);
	putInMemory(0xB000, 0x8512, 'l');
	putInMemory(0xB000, 0x8513, 0x7);
	putInMemory(0xB000, 0x8514, 'd');
	putInMemory(0xB000, 0x8515, 0x7);
	while(1){
	}
	return 0;
}

void printString(char *chars){
	char c = *chars;
	while(c != '\0'){
	
	}
}
