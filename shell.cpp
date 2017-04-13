#include<windows.h>
#include<stdio.h>
#include<string.h>
#include<iostream>
using namespace std;

TCHAR szFilename[10][MAX_PATH] = {  "mycat.exe",
									"myhead.exe",
									"mysort.exe"
									};	//Store .exe path
HANDLE hCmd[10] = { INVALID_HANDLE_VALUE };	//Array that stores the handle of process
PROCESS_INFORMATION pi[10] = { NULL }; //Array that stores the structure variable of process
int counter_child_process = 0;	//The number of created process

struct CMD{                 //e.g:"myhead 3 file1 | mycat -n"
	int cmdposi[10];		//cmdposi[0] = 0,cmdposi[1] = 4
	int lastcmdposi;		//lastcmdposi = 1 
	int subcmdparameter[10];//subcmdparameter[0] = 2, subcmdparameter[1] = 1
	char* subcmd[10][10];		/*subcmd[0]: myhead 3 file1     subcmd[1]: mycat -n
							subcmd[0][0] = myhead, subcmd[0][1] = 3, subcmd[0][2] = file1
							subcmd[1][0] = mycat, subcmd[1][1] = -n   */
}cmd;

void seperate_cmd(char* cmdline[]);
HANDLE Create_Cmd_Process(char* (*arr)[10],int cmd_row,char *cmdtype,int islastcmd);
void create_mytime(char* cmdline[]);
void create_myls(char* cmdline[]);
void create_mycp(char* cmdline[]);
void ShowRunTime(SYSTEMTIME a,SYSTEMTIME b);
void create_mysl(char* cmdline[]);

int main(int argc,char* argv[]){
	while(1){
		HANDLE hstd = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_BLUE);
		printf("Please input command:");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY
							| FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		//system("color x1x2");
		//system("color 0E");
		//system("color 07");
		int cmdwordNum = 0;
		char arr[10][200] = {0};
		char* cmdword[10] = {NULL};
		int i;
		for(i=0; i<10; i++){
			char temp[200] = {0};
			int j = 0;
			char c;
			while(1){
				scanf("%c",&c);
				if(c == ' '||c == '\n')
					break;
				else
					temp[j++] = c;
			}
			strcpy(arr[i],temp);
			cmdword[i] = &arr[i][0];
			if(c == '\n')
				break;
		}
		cmdwordNum = ++i;

		if(strcmp(cmdword[0],"mytime")==0)
			create_mytime(cmdword);
		else if(strcmp(cmdword[0],"myls")==0)
			create_myls(cmdword);
		else if(strcmp(cmdword[0],"mysl")==0)
			create_mysl(cmdword);
		else if(strcmp(cmdword[0],"mycp")==0)
			create_mycp(cmdword);
		else{
			for(int i=0; i<10; i++)	{	//init cmd.cmdposi[] ¡¢ cmd.lastcmdposi and cmd.subcmdparameter[]
				cmd.cmdposi[i] = -1;
				cmd.subcmdparameter[i] = -1;
			}
			cmd.lastcmdposi = -1;
			for(int i=0; i<cmdwordNum; i++){   //fill cmd.cmdposi[] and cmd.lastcmdposi
				if (   strcmp(cmdword[i],"myhead")==0  ||
						strcmp(cmdword[i],"mycat")==0  ||
						strcmp(cmdword[i],"mysort")==0    ){
					cmd.cmdposi[++cmd.lastcmdposi]=i;
				}
			}
			int j;								//fill cmd.subcmdparameter[]
			for(j=0; j<cmd.lastcmdposi; j++)
				cmd.subcmdparameter[j] = cmd.cmdposi[j+1]-cmd.cmdposi[j]-2;
			cmd.subcmdparameter[j] = cmdwordNum-cmd.cmdposi[j]-1;
			seperate_cmd(cmdword);	//fill subcmd[]

			int islastcmd = FALSE;
			for(int i=0; i<=cmd.lastcmdposi; i++){  	//Create child process of each subcmd
				if(i == cmd.lastcmdposi)
					islastcmd = TRUE;
				hCmd[i] = Create_Cmd_Process(cmd.subcmd, i, cmdword[cmd.cmdposi[i]], islastcmd);
				if(hCmd[i] == INVALID_HANDLE_VALUE){
					printf("Create process %s failed !!!\n",cmdword[cmd.cmdposi[i]]);
				}
				//else
				//	printf("Create process %s successfully\n",cmdword[cmd.cmdposi[i]]);
			}

			for(int i=0; i<=cmd.lastcmdposi;i++){		//Terminate each process
				if(hCmd[i] == NULL)
					printf("hCmd[%d] is NULL !!!\n",i);

				if( WaitForSingleObject(hCmd[i],INFINITE) == WAIT_FAILED)
					printf("Wait for single object failed !!!%d\n",GetLastError());

				if(!(CloseHandle(pi[i].hProcess) && CloseHandle(pi[i].hThread)) )
					printf("Terminate %s failed !!!\n",cmdword[cmd.cmdposi[i]]);
				//else
				//	printf("Terminate %s successfully\n",cmdword[cmd.cmdposi[i]]);
			}

			//Clear all global data
			for(int i=0; i<10; i++)	{	//init cmd.cmdposi[] ¡¢ cmd.lastcmdposi and cmd.subcmdparameter[]
				//Clear cmd
				cmd.cmdposi[i] = -1;
				cmd.subcmdparameter[i] = -1;
				for(int j=0; j<10; j++)
					cmd.subcmd[i][j] = NULL;
				//Clear others
				hCmd[i] = INVALID_HANDLE_VALUE;

			}
			cmd.lastcmdposi = -1;
			counter_child_process = 0;
			memset(pi,0,sizeof(pi));
		}//end else
	}//end while
	return 0;
}

///*
void seperate_cmd(char* cmdline[]){
	//printf("%s\n%s\n",cmdline[0],cmdline[1]);
	//cmd.subcmd[0][0] = cmdline[0];
	//printf("%s\n",cmd.subcmd[0][0]);
	for(int i=0; i<=cmd.lastcmdposi; i++){
		//printf("subcmd[%d]: ",i);
		for(int j=0; j<=cmd.subcmdparameter[i]; j++){
			cmd.subcmd[i][j] = cmdline[cmd.cmdposi[i]+j];
			//printf("%s ",cmd.subcmd[i][j]);
		}
		//printf("\n");
	}
}
//*/

HANDLE Create_Cmd_Process(char* (*arr)[10],int cmd_row,char *cmdtype,int islastcmd){
	//printf("Subcmdline is: ");
	//for(int i=0;arr[cmd_row][i]!=NULL; i++)
	//	printf("%s ",arr[cmd_row][i]);
	//printf("\n");

	TCHAR szCmdLine[MAX_PATH];
	if(strcmp(cmdtype,"mycat")==0)
		sprintf(szCmdLine,"\"%s\" %s %s %s %s %d",szFilename[0],arr[cmd_row][1],
					arr[cmd_row][2],arr[cmd_row][3],arr[cmd_row][4],islastcmd);
	else if(strcmp(cmdtype,"myhead")==0)
		sprintf(szCmdLine,"\"%s\" %s %s %s %s %d",szFilename[1],arr[cmd_row][1],
					arr[cmd_row][2],arr[cmd_row][3],arr[cmd_row][4],islastcmd);
	else if(strcmp(cmdtype,"mysort")==0)
		sprintf(szCmdLine,"\"%s\" %s %s %s %s %d",szFilename[2],arr[cmd_row][1],
					arr[cmd_row][2],arr[cmd_row][3],arr[cmd_row][4],islastcmd);

	STARTUPINFO si;	
	ZeroMemory(reinterpret_cast<void *>(&si),sizeof(si));
	si.cb = sizeof(si);

	BOOL bCreateOK = CreateProcess(
		NULL,				//.exe path
		szCmdLine,			//cmd line
		NULL,				//Default process safety
		NULL,				//Default thread safety
		FALSE,				//Not inherit the handle of opened file
		NULL,				//Not using new console
		NULL,				//New environment
		NULL,				//Current dir
		&si,				//Startup information
		&pi[counter_child_process]	);		//Retrive info about process and thread

	//Return handle of process
	if(bCreateOK){
		return pi[counter_child_process++].hProcess;
	}
}

//Show runtime
void ShowRunTime(SYSTEMTIME a,SYSTEMTIME b){
	long total_time = 0;	//in millisecond
	int minutes = 0, seconds = 0, milliseconds= 0;
	total_time = (b.wMinute - a.wMinute)*60000 +(b.wSecond - a.wSecond)*1000 +(b.wMilliseconds - a.wMilliseconds);
	minutes = total_time/60000;
	seconds = (total_time - 60000 * minutes)/1000;
	milliseconds = total_time - 60000 * minutes - 1000 * seconds;
	printf("It takes %d minutes %d seconds %d milliseconds \n",minutes,seconds,milliseconds);
}

//mytime program1.exe
void create_mytime(char* cmdline[]){
	TCHAR szFilename[MAX_PATH];
	TCHAR szCmdLine[MAX_PATH];
	SYSTEMTIME Start,Finish;

	strcpy(szFilename,cmdline[1]);

	//printf("cmdline[0] = %s\n",cmdline[0]);	//cmdline[0] = mytime
	//printf("cmdline[1] = %s\n",cmdline[1]);	//cmdline[1] = program1.exe
	sprintf(szCmdLine,"\"%s\"",szFilename);

	STARTUPINFO si;	
	ZeroMemory(reinterpret_cast<void *>(&si),sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi_mytime;
	//Create a process for .exe file
	GetSystemTime(&Start);
	BOOL bCreateOK = CreateProcess(
		NULL,				//.exe path
		szCmdLine,			//cmd line
		NULL,				//Default process safety
		NULL,				//Default thread safety
		FALSE,				//Not inherit the handle of opened file
		NULL,				//Not using new console
		NULL,				//New environment
		NULL,				//Current dir
		&si,				//Startup information
		&pi_mytime);		//Retrive info about process and thread

	HANDLE hChild = pi_mytime.hProcess;

	if(hChild != INVALID_HANDLE_VALUE){  
		if( WaitForSingleObject(hChild,INFINITE) == WAIT_FAILED)
			printf("Wait for single object failed !!!%d\n",GetLastError());
		GetSystemTime(&Finish);		
		ShowRunTime(Start , Finish);

		if(!(CloseHandle(pi_mytime.hProcess) && CloseHandle(pi_mytime.hThread)) )
			printf("Terminate %s failed !!!\n",cmdline[1]);
		//else
		//	printf("Terminate successfully\n");
	}
}

void create_mycp(char* cmdline[]){
	TCHAR szFilename[MAX_PATH] = "mycp.exe";
	TCHAR szCmdLine[MAX_PATH];
	sprintf(szCmdLine,"\"%s\" %s %s",szFilename,cmdline[1],cmdline[2]);

	STARTUPINFO si;	
	ZeroMemory(reinterpret_cast<void *>(&si),sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi_mycp;

	BOOL bCreateOK = CreateProcess(
		NULL,				//.exe path
		szCmdLine,			//cmd line
		NULL,				//Default process safety
		NULL,				//Default thread safety
		FALSE,				//Not inherit the handle of opened file
		CREATE_NEW_CONSOLE,				//Using new console
		NULL,				//New environment
		NULL,				//Current dir
		&si,				//Startup information
		&pi_mycp);		//Retrive info about process and thread

	HANDLE hChild = pi_mycp.hProcess;

	if(hChild != INVALID_HANDLE_VALUE){  
		if( WaitForSingleObject(hChild,INFINITE) == WAIT_FAILED)
			printf("Wait for single object failed !!!%d\n",GetLastError());
		if(!(CloseHandle(pi_mycp.hProcess) && CloseHandle(pi_mycp.hThread)) )
			printf("Terminate %s failed !!!\n",cmdline[0]);
	}
}

void create_myls(char* cmdline[]){
	TCHAR szFilename[MAX_PATH] = "myls.exe";
	TCHAR szCmdLine[MAX_PATH];
	sprintf(szCmdLine,"\"%s\" %s",szFilename,cmdline[1]);

	STARTUPINFO si;	
	ZeroMemory(reinterpret_cast<void *>(&si),sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi_myls;

	BOOL bCreateOK = CreateProcess(
		NULL,				//.exe path
		szCmdLine,			//cmd line
		NULL,				//Default process safety
		NULL,				//Default thread safety
		FALSE,				//Not inherit the handle of opened file
		NULL,				//Not using new console
		NULL,				//New environment
		NULL,				//Current dir
		&si,				//Startup information
		&pi_myls);		//Retrive info about process and thread

	HANDLE hChild = pi_myls.hProcess;

	if(hChild != INVALID_HANDLE_VALUE){  
		if( WaitForSingleObject(hChild,INFINITE) == WAIT_FAILED)
			printf("Wait for single object failed !!!%d\n",GetLastError());
		if(!(CloseHandle(pi_myls.hProcess) && CloseHandle(pi_myls.hThread)) )
			printf("Terminate %s failed !!!\n",cmdline[1]);
		//else
		//	printf("Terminate successfully\n");
	}
}

void create_mysl(char* cmdline[]){
	TCHAR szFilename[MAX_PATH] = "mysl.exe";
	TCHAR szCmdLine[MAX_PATH];
	sprintf(szCmdLine,"\"%s\"",szFilename);

	STARTUPINFO si;	
	ZeroMemory(reinterpret_cast<void *>(&si),sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi_mysl;

	BOOL bCreateOK = CreateProcess(
		NULL,				//.exe path
		szCmdLine,			//cmd line
		NULL,				//Default process safety
		NULL,				//Default thread safety
		FALSE,				//Not inherit the handle of opened file
		CREATE_NEW_CONSOLE,
		NULL,				//New environment
		NULL,				//Current dir
		&si,				//Startup information
		&pi_mysl);		//Retrive info about process and thread

	HANDLE hChild = pi_mysl.hProcess;

	if(hChild != INVALID_HANDLE_VALUE){
		if( WaitForSingleObject(hChild,INFINITE) == WAIT_FAILED)
			printf("Wait for single object failed !!!%d\n",GetLastError());
		if(!(CloseHandle(pi_mysl.hProcess) && CloseHandle(pi_mysl.hThread)) )
			printf("Terminate %s failed !!!\n",cmdline[0]);
	}
}