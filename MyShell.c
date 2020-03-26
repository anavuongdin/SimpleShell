#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdbool.h>
#include<windows.h>
#include<process.h>
#define LENGTH 100
char command[LENGTH];
int startup = 1;
void StartupScreen(){
	//*Need to edit
	printf("Here is startup screen!");
	printf("\n1.");
	printf("\n2.");
	printf("\n***********");
}
int WaitForUserEnd(){
	int i;
	printf("\n>>");
	fflush(stdin);
	gets(command);
	for(i=0; i<LENGTH; i++)
		if(command[i]=='\0')
			return 1;
	return 0;
}
void PrintCommand(){
	printf("\nYour command is %s", command);
}
void Error(int type){
	if(type == 0)
		printf("Command exceeds maximum length.");
}
void ExitShell(){
	exit(0);
}
int RunFProcess(char *process_name){
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	if(!CreateProcess(process_name
					 ,NULL
					 ,NULL
					 ,NULL
					 ,FALSE
					 ,0
					 ,NULL
					 ,NULL
					 ,&si
					 ,&pi)){
		printf("Create process failed! (%d)", GetLastError());
		return 0;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 1;
}
int main(){
	char process_name[LENGTH];
	while(1){
		if(startup){
			StartupScreen();
			startup = 0;
		}
		if(WaitForUserEnd()){
			PrintCommand();
			if(!strcmp(command, "exit"))
				ExitShell();
			else if(!strcmp(command, "run")){
				printf("\nEnter name of program you want to run: ");
				fflush(stdin);
				gets(process_name);
				RunFProcess(process_name);
				getch();
			}
			system("cls");
		}
		else
			Error(0);
	}
	
}
