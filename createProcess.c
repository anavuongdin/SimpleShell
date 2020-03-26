#include<windows.h>
#include<stdio.h>
int main(int argc, char **argv){
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	if(argc !=2){
		printf("Usage %s [cmdline]\n", argv[0]);
		return 0;
	}
	if(!CreateProcess(argv[1]
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
	printf("Creating a process...\n");
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 0;
}
