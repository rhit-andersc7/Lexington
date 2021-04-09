#include "shell.h"

int main(){

	char prompt[5];
	char line[120];

	prompt[0]='L';
	prompt[1]='>';
	prompt[2]=' ';
	prompt[3]=' ';
	prompt[4]='\0';

	while(1){
		interrupt(0x21, 0, prompt, 0, 0);
		interrupt(0x21, 1, line, 0, 0);
		runCommand(line); 
	}

}

void runCommand(char* line){	

}

void typeCommand(char* filename){	
	interrupt(0x21, 3, filename, 0x2000, 0);	
}

void executeCommand(char* filename){
	interrupt(0x21, 4, filename, 0x2000, 0);	
}

void dirCommand(){
/*        for(i = 0; i < 16; i++){
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
                                j++;
                        }

                        return (j-5)*512;
                }
        }
*/
}

