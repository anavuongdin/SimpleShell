#include "setup.h"
int main(){
	int startup = 1;
	while(1){
		if(startup){
			StartupScreen();
			startup = 0;
		}
		else if(WaitForUserEnd())
			ParseCommand();
	}
}
