/*
myhead.cpp
If argc = 3, then argv[1] is a number,argv[2] is a filepath. This is a sole using. 
Sample:  myhead.exe 3 file
*/
#include<windows.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
using namespace std;

//Generic parameters
int LeftMycatPipeFlag=0;	//Check the existence of left pipe
int LeftMysortPipeFlag=0;
int islastcmd = FALSE;
char buffer[1024] ={ NULL };	//Pipe buffer
DWORD ReadNum;
DWORD WriteNum;

//Exclusive parameters
int filenum;
int headnum = 0;	//First parameter
char **filepath = NULL;	//Seecond parameter


int string_to_num(char *str) {
	int i=0,sum=0;
	while(str[i] != '\0') {
		sum=sum*10+str[i]-48;
		i++;
	}
	return sum;
}

int find_left_mycat_pipe(){
	if(WaitNamedPipe("\\\\.\\Pipe\\mycat",NMPWAIT_WAIT_FOREVER)==FALSE){    //Waiting for mycat's pipe
		//printf("Myhead waits for mycat's pipe failed !!!\n");
		return FALSE;
	}
	else {
		//printf("Myhead waits for mycat's pipe successfully\n");
		return TRUE;
	}
}

void get_left_mycat_pipe(){
	HANDLE hpipe = CreateFile("\\\\.\\Pipe\\mycat",
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(hpipe == INVALID_HANDLE_VALUE){
		printf("Myhead opens mycat's pipe failed !!!\n");
		return ;
	}
	else {	
		//printf("Myhead opens mycat's pipe successfully\n");
		if( ReadFile(hpipe,buffer,1024,&ReadNum,NULL)==0 )
			printf("Myhead reads mycat's pipe failed !!!\n");
		else {
			//printf("Myhead reads mycat's pipe successfully\n");
			buffer[ReadNum] = 0;

			int row = 0;							//show data by line divided from buffer
			int row_member_num[200] = {0};
			int j = 0;
			for(int i=0; buffer[i]!='\0';i++){
				if(buffer[i] == '\n'){
					row_member_num[row] = j;
					//printf("row_member_num[%d] = %d\n",row,row_member_num[row]);
					j = 0;
					row ++;
					continue;
				}
				j++;
			}
			row_member_num[row] = j;
			//printf("row_member_num[%d] = %d\n",row,row_member_num[row]);
			row++;
			int posi = 0;
			printf("    After myhead:\n");
			for(int i=0; i<headnum; i++){
				char temp[200] = {0};
				for(int j=0; j<row_member_num[i]; j++){
					temp[j] = buffer[posi];
					posi ++;
				}
				posi++;
				printf("%s\n",temp);
			}
			for(int i=posi-1; i<1024; i++)
				buffer[i] = 0;

		}
	}//end big else
	CloseHandle(hpipe);
}

int find_left_mysort_pipe(){
	if(WaitNamedPipe("\\\\.\\Pipe\\mysort",NMPWAIT_WAIT_FOREVER)==FALSE){    //Waiting for mysort's pipe
		//printf("Myhead waits for mysort's pipe failed !!!\n");
		return FALSE;
	}
	else {
		//printf("Myhead waits for mysort's pipe successfully\n");
		return TRUE;
	}
}

void get_left_mysort_pipe(){
	HANDLE hpipe = CreateFile("\\\\.\\Pipe\\mysort",
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(hpipe == INVALID_HANDLE_VALUE){
		printf("Myhead opens mysort's pipe failed !!!\n");
		return ;
	}
	else {
		//printf("Myhead opens mysort's pipe successfully\n");
		if( ReadFile(hpipe,buffer,1024,&ReadNum,NULL)==0 )
			printf("Myhead reads mysort's pipe failed !!!\n");
		else {
			//printf("Myhead reads mysort's pipe successfully\n");
			buffer[ReadNum] = 0;

			//operation----------------------------------------------------------
			int row = 0;							//show data by line divided from buffer
			int row_member_num[200] = {0};
			int j = 0;
			for(int i=0; buffer[i]!='\0';i++){
				if(buffer[i] == '\n'){
					row_member_num[row] = j;
					//printf("row_member_num[%d] = %d\n",row,row_member_num[row]);
					j = 0;
					row ++;
					continue;
				}
				j++;
			}
			row_member_num[row] = j;
			//printf("row_member_num[%d] = %d\n",row,row_member_num[row]);
			row++;
			int posi = 0;
			printf("    After myhead:\n");
			for(int i=0; i<headnum; i++){
				char temp[200] = {0};
				for(int j=0; j<row_member_num[i]; j++){
					temp[j] = buffer[posi];
					posi ++;
				}
				posi++;
				printf("%s\n",temp);
			}
			for(int i=posi-1; i<1024; i++)
				buffer[i] = 0;
		}
	}//end big else
	CloseHandle(hpipe);
}

void create_myhead_pipe(){
	HANDLE hpipe = CreateNamedPipe(	//	Create pipe
		"\\\\.\\Pipe\\myhead",
		PIPE_ACCESS_DUPLEX,	// the pipe is bidirectional
		PIPE_TYPE_BYTE|PIPE_READMODE_BYTE,
		1,
		0,
		0,
		1000,
		NULL
		);

	if(hpipe == INVALID_HANDLE_VALUE)
		printf("Myhead creates myhead's pipe failed !!!\n");
	//else 
		//printf("Myhead creates myhead's pipe successfully\n");

	if(ConnectNamedPipe(hpipe,NULL) == FALSE){			//Waiting for connection
		CloseHandle(hpipe);
		printf("No one connects myhead's pipe !!!\n");
	}
	else {
		//printf("Someone connects myhead's pipe\n");		

		if( WriteFile(hpipe,buffer,strlen(buffer),&WriteNum,NULL)==FALSE )	
			printf("Myhead writes into myhead's pipe failed !!!\n");
		//else
		//	printf("%s\n",buffer);
	}
	
	if(DisconnectNamedPipe(hpipe) == FALSE)				//Disconnect
		printf("Myhead disconnects failed !!!\n");
	//else
	//	printf("Myhead disconnects successfully\n");
}

void process_file_print(){
	//int filenum = 1;
	vector<string> data;	//Store data by line
	string line;
	printf("    Before myhead:\n");
	for(int i=0; i<filenum; i++){
		ifstream in(filepath[i]);
		while(getline(in,line)){
			cout<<line<<endl;
			data.push_back(line);
		}
	}
	int i;
	for(i=0; i<headnum-1; i++){
		strcat(buffer,data[i].c_str());
		strcat(buffer,"\n");
	}
	strcat(buffer,data[i].c_str());
	printf("    After myhead:\n");
	printf("%s\n",buffer);
}

// Process file then put data into buffer
void process_file_into_buffer(){	
	vector<string> data;	//Store data by line
	string line;
	printf("    Before myhead:\n");
	for(int i=0; i<filenum; i++){
		ifstream in(filepath[i]);
		while(getline(in,line)){
			cout<<line<<endl;
			data.push_back(line);
		}
	}
	int i;
	for(i=0; i<headnum-1; i++){
		strcat(buffer,data[i].c_str());
		strcat(buffer,"\n");
	}
	strcat(buffer,data[i].c_str());
	printf("    After myhead:\n");
	printf("%s\n",buffer);
}

int main(int argc,char*argv[]){
	filenum = argc-2;
	headnum = string_to_num(argv[1]);
	filepath = &argv[2];
	islastcmd = string_to_num(argv[5]);
	
	//printf("myhead.exe argc=%d ----- ",argc);
	//printf("argv[1]=%s,argv[2]=%s,argv[3]=%s,argv[4]=%s,argv[5]=%s\n",argv[1],argv[2],argv[3],argv[4],argv[5]);
	
	LeftMycatPipeFlag = find_left_mycat_pipe();
	LeftMysortPipeFlag = find_left_mysort_pipe();
	int existence = LeftMycatPipeFlag || LeftMysortPipeFlag;	//	Judge the existence of left pipe
	if(existence==1 && islastcmd==0){       //eg: mycat -n file1.txt | myhead 3 | mysort 
		//get data from pipe
		if(LeftMycatPipeFlag == 1)
			get_left_mycat_pipe();
		else if(LeftMysortPipeFlag == 1)
			get_left_mysort_pipe();
		//create pipe, place data
		create_myhead_pipe();
	}
	else if(existence==0 && islastcmd==1){  //eg: myhead 3 file1.txt
		//process data ,then print
		process_file_print();
	}
	else if(existence==1 && islastcmd==1){  //eg: mycat -n file1.txt | myhead 3
		//get data from pipe,then print
		if(LeftMycatPipeFlag == 1)
			get_left_mycat_pipe();
		else if(LeftMysortPipeFlag == 1)
			get_left_mysort_pipe();
	}
	else if(existence==0 && islastcmd==0){  //eg: myhead 3 file1.txt | mycat -n 
		//process data ,put into buffer
		process_file_into_buffer();
		//create pipe, place data
		create_myhead_pipe();
	}

	/*
	if(argc == 3){	//myhead carries complete parameters
		int headnum = string_to_num(argv[1]);
		char *filepath = argv[2];

		ifstream in(filepath);
		string line;
		for(int i=0; i<headnum; i++){
			getline(in,line);
			cout<<line<<endl;
		}
	}
	*/
	return 0;
}