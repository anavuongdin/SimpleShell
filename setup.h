#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<windows.h>
#include<process.h>
#include<dirent.h>
#include<time.h>
#include "feature.h"
#define LENGTH 100
#define ARGLENGTH 3
#define PROLENGTH 10

char path[LENGTH] = "";						// Path currently used, default path is the path contains MyShell.exe
char command[LENGTH]; 						// Prompt user sends
char argument[ARGLENGTH][LENGTH]; 			// Store aruguments passed
char dir[LENGTH]; 							// Directory passed, allow space character
int arg_count; 								// Store number of arguments passed
PROCESS_INFORMATION pi[PROLENGTH]; 			// Store information of existing process
int process_count=0; 						// Store number of existing process
char process_list[PROLENGTH][LENGTH]; 		// Name of existing process
char process_status[PROLENGTH][LENGTH]; 	// Status of existing process

int IsRunning(PROCESS_INFORMATION pi){
	DWORD status = WaitForSingleObject(pi.hProcess, 10);	
	if(status == WAIT_OBJECT_0) 			// Process has exited
		return 0;
	else if(status == WAIT_TIMEOUT)
		return 1;
	else return -1;
}

void List(){
	int j;
	printf("Process Name           PID  STATUS");
	printf("\n====================== === ========");
	if(process_count>0){
		for (j=0; j<process_count; j++){
			if(IsRunning(pi[j])==1){
			printf("\n%-22s %-3d %-8s",process_list[j],j,process_status[j]);}
		}
	}
	printf("\n");
}

void Error(int type){
	switch(type){
		case 0:
			printf("Command exceeds maximum length.");
			break;
		case 1:
			printf("Command was not recognized. Type <help> for further information.");
			break;
		case 2: 
			printf("Number of argument exceeds maximum length.");
			break;
		case 3:
			printf("Unable to create process, error code: %d.", GetLastError());
			break;
		case 4:
			printf("Could not open current directory." );
			break;
		case 5:
			printf("Must pass proper arguments.");
			break;
	}
	getch(); 
	printf("\n");
}

int RunBProcess(char *process_name){
	char temp_path[LENGTH]; //Used for locally storing path and changable 
	STARTUPINFO si;
	strcpy(temp_path, path);
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	if(!CreateProcess(strcat(temp_path, process_name) ,NULL ,NULL ,NULL 
					  ,FALSE ,CREATE_NEW_CONSOLE ,NULL ,NULL ,&si ,&pi[process_count])){
		Error(3);
		return 0;
	}
	strcpy(process_list[process_count],process_name);
	strcpy(process_status[process_count],"RUNNING");
	return 1;
}

int RunFProcess(char *process_name){
	char temp_path[LENGTH];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	strcpy(temp_path, path);
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	if(!CreateProcess(strcat(temp_path, process_name), NULL ,NULL
					  ,NULL ,FALSE ,0 ,NULL ,NULL ,&si,&pi)){
		Error(3);
		return 0;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 1;
}

int CheckDir(char *source){
	DIR *dr = opendir(source); 
    if (dr == NULL) {
    	Error(4);
    	return 0;
	}
	return 1;
}

void Dir(char *source) {
	struct dirent *de;  				// Pointer for directory entry
    DIR *dr = opendir(source);   		 
  	if(CheckDir(source)){				// opendir returns NULL if couldn't open directory
  		while ((de = readdir(dr)) != NULL) 
            printf("%s\n", de->d_name); 
   		closedir(dr); 
	  }    
}

void Kill(char *i){
	if(IsNumber(i)){
		TerminateProcess(pi[atoi(i)].hProcess, 0);
		CloseHandle(pi[atoi(i)].hThread);
	}	
	else
		Error(5);
}

void KillAll(){
	int i;
	char buffer[LENGTH];
	for (i = 0; i<process_count; i++)
		Kill(itoa(i, buffer, 10));
}

void Stop(char *i){
	if(IsNumber(i)){
		SuspendThread(pi[atoi(i)].hThread);	
		strcpy(process_status[atoi(i)],"PAUSED");
	}
	else
		Error(5);
}

void Resume(char *i){
	if(IsNumber(i)){
		ResumeThread(pi[atoi(i)].hThread);
		strcpy(process_status[atoi(i)],"RUNNING");	
	}
	else
		Error(5);
}

int WaitForUserEnd(){
	int i, j, k=0, l=0, p=0;
	int previous=0;
	printf("\n>>");
	fflush(stdin);
	gets(command);
	for(i=0; i<LENGTH; i++)
		if(command[i]=='\0'){					//Command accepted
			for(j=0; j<i;j++)
				if(command[j]!=' ')				//Find first character not space
					break;
			for(;j<=i; j++){
				if(k!=0)
					dir[p++] = command[j];
				if((command[j]!=' ') && (command[j] !='\0')){
					argument[k][l++] = command[j];
					previous=0;
					}
				else
					if(!previous){
						argument[k++][l] = '\0'; 	//Split into arguments passed
						l=0;
						previous=1;
					}	
			}
			arg_count = (!(strcmp(argument[0], "dir"))||!(strcmp(argument[0], "set")))? ((k==1)? 1:2): k; 
									//Number of argument passes, exception set and dir
			if(arg_count > ARGLENGTH){		//Exceeds length
				Error(2);
				return 0;
			}
			return 1;	
		}
	Error(0);
	return 0; 						//Command not accepted
}

void ExitShell(){
	printf("\nThank you for using MyShell.");
	KillAll();
	exit(0);
}

void ParseCommand(){
	if(!strcmp(argument[0], "exit")&&(arg_count==1))
		ExitShell();
	else if(!strcmp(argument[0], "run") &&(arg_count==3)){
		if(!strcmp(argument[1], "b")){
			if(RunBProcess(strcat(argument[2], ".exe")))
				process_count++;
		}	
		else if(!strcmp(argument[1], "f"))
			RunFProcess(strcat(argument[2], ".exe"));
		else 
			Error(1);
	}			
	else if(!strcmp(argument[0], "help")&&(arg_count==1))
		PrintHelp();
	else if(!strcmp(argument[0], "dir")&&(arg_count==2))
		Dir(dir);
	else if(!strcmp(argument[0], "set")){
		if(arg_count!=1)
			if(CheckDir(dir))
				strcpy(path, strcat(dir, "\\"));
		else if(arg_count==1)
			strcat(dir, "\\");
	}
	else if(!strcmp(argument[0], "list"))
		List();
	else if(!strcmp(argument[0], "kill")){
		if(!strcmp(argument[1], "o")&&(arg_count==3))
			Kill(argument[2]);
		else if(!strcmp(argument[1], "a")&&(arg_count==2))
			KillAll();
		else
			Error(1);
	}
	else if(!strcmp(argument[0], "pause")&&(arg_count==2))
		Stop(argument[1]);
	else if(!strcmp(argument[0], "resume")&&(arg_count==2))
		Resume(argument[1]);
	else if(!strcmp(argument[0], "time")&&(arg_count==1))
		TimeNow();
	else
		Error(1);
}
