void StartupScreen(){
	int i = 1;
	printf("\n");		
	printf("                                        !!! WELCOME TO MY SHELL !!!											");
	printf("\n                                              /   00000000   \\	");
	printf("\n                                             /  000000000000  \\	");
	printf("\n                                            / 0000000000000000 \\");
	printf("\n                                           |  0000        0000  |");
	printf("\n                                           |  0    ©©©©©©    0  |");
	printf("\n                                           |    ©©©©©©©©©©©©    |");
	printf("\n                                            \\     ©©©©©©©©     /");
	printf("\n                                             \\       ©©       /");
	printf("\n                                              \\      ©©      /");
	printf("\n");
	printf("\n\t My shell supports the following commands:");
	printf("\n\t\t %-2d. dir			: List all files in this directory", i++);
	printf("\n\t\t %-2d. run f		: Run program in forgeground", i++);
	printf("\n\t\t %-2d. run b		: Run program in background", i++);
	printf("\n\t\t %-2d. list		: List the process id is running", i++);
	printf("\n\t\t %-2d. kill o		: Kill process with id is running", i++);
	printf("\n\t\t %-2d. kill a		: Kill all process are running", i++);
	printf("\n\t\t %-2d. stop		: Stop a running process", i++);
	printf("\n\t\t %-2d. resume		: Resume a stopped process", i++);
	printf("\n\t\t %-2d. time		: Print time and date", i++);
	printf("\n\t\t %-2d. help		: Help users", i++);
	printf("\n\t\t %-2d. exit		: Exit Shell", i++);	
	getch();
	system("cls");	
}
void PrintHelp(){
	printf("\nMyShell supports the following commands:");
	printf("\n1. set <path_name>");
	printf("\nThis command sets the path to <path_name>, if there is no parameter, the path is considered as default.");
	printf("\nBackground process can only be run by this path.");
	printf("\n\n2. dir <path_name>");
	printf("\nThis command lists all file in directory <path_name>.");
	printf("\n\n3. run f <program_name>");
	printf("\nThis command runs program in forgeground.");
	printf("\n\n4. run b <program_name>");
	printf("\nThis command runs program in background.");
	printf("\nThe program must be in 'path' environment variable.");
	printf("\n\n5. kill o <pid>");
	printf("\nThis command kills a process by using its pid.");
	printf("\n\n6. kill a");
	printf("\nThis command kills all running process.");
	printf("\n\n7. pause <pid>");
	printf("\nThis command stops a running process by using its pid");
	printf("\n\n8. resume <pid>");
	printf("\nThis command resumes a stopped process by using its pid");
}
int IsNumber(char *s){
	int i;
	for(i=0; i<strlen(s); i++)
		if(!isdigit(s[i]))
			return 0;
	return 1;
}
void TimeNow(){
	int hours, minutes, seconds, day, month, year;  // variables to store date and time components
	time_t now; 									// time_t is arithmetic time type
	time(&now); 				// Obtain current time
								// time() returns the current time of the system as a time_t value
								// Convert to local time format and print to stdout
	printf("Today is : %s", ctime(&now));
								// localtime converts a time_t value to calendar time and 
								// returns a pointer to a tm structure with its members 
								// filled with the corresponding values
	struct tm *local = localtime(&now);
    hours = local->tm_hour;      	// get hours since midnight (0-23)
    minutes = local->tm_min;     	// get minutes passed after the hour (0-59)
    seconds = local->tm_sec;     	// get seconds passed after minute (0-59)
    day = local->tm_mday;        	// get day of month (1 to 31)
    month = local->tm_mon + 1;   	// get month of year (0 to 11)
    year = local->tm_year + 1900;	// get year since 1900
	if (hours < 12)	// before midday
		printf("Time is : %02d:%02d:%02d am\n", hours, minutes, seconds);		// print local time
	else	
		printf("Time is : %02d:%02d:%02d pm\n", hours - 12, minutes, seconds);	// after midday
	printf("Date is : %02d/%02d/%d\n", day, month, year);						// print current date
}
