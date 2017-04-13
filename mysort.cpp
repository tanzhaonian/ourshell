/*
mysort.cpp
Sample:  mysort.exe file1 file2 file3
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
int LeftMyheadPipeFlag=0;
int islastcmd = FALSE;
char buffer[1024] ={ NULL };	//Pipe buffer
DWORD ReadNum;
DWORD WriteNum;

//Exclusive parameters
int filenum;
char **filepath = NULL;	//First parameter

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
		//printf("Mysort waits for mycat's pipe failed !!!\n");
		return FALSE;
	}
	else {
		//printf("Mysort waits for mycat's pipe successfully\n");
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
		printf("Mysort opens mycat's pipe failed !!!\n");
		return ;
	}
	else {	
		//printf("Mysort opens mycat's pipe successfully\n");
		if( ReadFile(hpipe,buffer,1024,&ReadNum,NULL)==0 )
			printf("Mysort reads mycat's pipe failed !!!\n");
		else {
			//printf("Mysort reads mycat's pipe successfully\n");
			buffer[ReadNum] = 0;
			//operation
			vector<string> data;
			string line;
			int row = 0;
			int row_member_num[200] = {0};
			int j = 0;
			for(int i=0; buffer[i]!='\0';i++){
				if(buffer[i] == '\n'){
					row_member_num[row] = j;
					j = 0;
					row ++;
					continue;
				}
				j++;
			}
			row_member_num[row] = j;
			row++;
			int posi = 0;
			for(int i=0; i<row; i++){
				char temp[200] = {0};
				for(int j=0; j<row_member_num[i]; j++){
					temp[j] = buffer[posi];
					posi ++;
				}
				posi++;
				string line(temp);
				data.push_back(line);
			}
			sort(data.begin(),data.end());
			for(int i=0; i<1024; i++)
				buffer[i] = 0;
			printf("    After mysort:\n");
			int i;
			for(i=0; i<data.size()-1; i++){
				strcat(buffer,data[i].c_str());
				strcat(buffer,"\n");
				//cout<<data[i]<<endl;
				printf("%s\n",data[i].c_str());
			}
			strcat(buffer,data[i].c_str());
			//cout<<data[i]<<endl;
			printf("%s\n",data[i].c_str());
			//end operation
		}
	}//end big else
	CloseHandle(hpipe);
}

int find_left_myhead_pipe(){
	if(WaitNamedPipe("\\\\.\\Pipe\\myhead",NMPWAIT_WAIT_FOREVER)==FALSE){    //Waiting for myhead's pipe
		//printf("Mysort waits for myhead's pipe failed !!!\n");
		return FALSE;
	}
	else {
		//printf("Mysort waits for myhead's pipe successfully\n");
		return TRUE;
	}
}

void get_left_myhead_pipe(){
	HANDLE hpipe = CreateFile("\\\\.\\Pipe\\myhead",
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(hpipe == INVALID_HANDLE_VALUE){
		printf("Mysort opens myhead's pipe failed !!!\n");
		return ;
	}
	else {		
		//printf("Mysort opens myhead's pipe successfully\n");
		if( ReadFile(hpipe,buffer,1024,&ReadNum,NULL)==0 )
			printf("Mysort reads myhead's pipe failed !!!\n");
		else {
			//printf("Mysort reads myhead's pipe successfully\n");
			buffer[ReadNum] = 0;
			
			//operation-----------------------------------------------------------------
			vector<string> data;
			string line;
			int row = 0;
			int row_member_num[200] = {0};
			int j = 0;
			for(int i=0; buffer[i]!='\0';i++){
				if(buffer[i] == '\n'){
					row_member_num[row] = j;
					j = 0;
					row ++;
					continue;
				}
				j++;
			}
			row_member_num[row] = j;
			row++;
			int posi = 0;
			for(int i=0; i<row; i++){
				char temp[200] = {0};
				for(int j=0; j<row_member_num[i]; j++){
					temp[j] = buffer[posi];
					posi ++;
				}
				posi++;
				string line(temp);
				data.push_back(line);
			}
			sort(data.begin(),data.end());
			for(int i=0; i<1024; i++)
				buffer[i] = 0;
			printf("    After mysort:\n");
			int i;
			for(i=0; i<data.size()-1; i++){
				strcat(buffer,data[i].c_str());
				strcat(buffer,"\n");
				cout<<data[i]<<endl;
			}
			strcat(buffer,data[i].c_str());
			cout<<data[i]<<endl;

		}
	}//end big else
	CloseHandle(hpipe);
}

void create_mysort_pipe(){
	HANDLE hpipe = CreateNamedPipe(	//	Create pipe
		"\\\\.\\Pipe\\mysort",
		PIPE_ACCESS_DUPLEX,	// the pipe is bidirectional
		PIPE_TYPE_BYTE|PIPE_READMODE_BYTE,
		1,
		0,
		0,
		1000,
		NULL
		);

	if(hpipe == INVALID_HANDLE_VALUE)
		printf("Mysort creates mysort's pipe failed !!!\n");
	//else 
	//	printf("Mysort creates mysort's pipe successfully\n");

	if(ConnectNamedPipe(hpipe,NULL) == FALSE){			//Waiting for connection
		CloseHandle(hpipe);
		printf("No one connects mysort's pipe !!!\n");
	}
	else {
		//printf("Someone connects mysort's pipe\n");		

		if( WriteFile(hpipe,buffer,strlen(buffer),&WriteNum,NULL)==FALSE )	
			printf("Mysort writes into myhead's pipe failed !!!\n");
		//else
		//	printf("Mysort writes into myhead's pipe successfully\n");
	}
	
	if(DisconnectNamedPipe(hpipe) == FALSE)				//Disconnect
		printf("Mysort disconnects failed !!!\n");
	//else
	//	printf("Mysort disconnects successfully\n");
}

void process_file_print(){
	//int filenum = 1;
	vector<string> data;	//Store data
	string line;
	printf("    Before mysort:\n");
	for(int i=0; i<filenum; i++){
		ifstream in(filepath[i]);
		while(getline(in,line)){
			cout<<line<<endl;
			data.push_back(line);
		}
	}
	sort(data.begin(),data.end());
	printf("    After mysort:\n");
	for(int i=0; i<data.size(); i++){
		cout<<data[i]<<endl;
	}
}

// Process file then put data into buffer--------------------------------------------------
void process_file_into_buffer(){	
	vector<string> data;	//Store data by line
	string line;
	printf("    Before mysort:\n");
	for(int i=0; i<filenum; i++){
		ifstream in(filepath[i]);
		while(getline(in,line)){
			cout<<line<<endl;
			data.push_back(line);
		}
	}
	sort(data.begin(),data.end());
	int i;
	for(i=0; i<data.size()-1; i++){
		strcat(buffer,data[i].c_str());
		strcat(buffer,"\n");
	}
	strcat(buffer,data[i].c_str());
	printf("    After mysort:\n");
	printf("%s\n",buffer);
}

int main(int argc,char* argv[]){

	filenum = argc-2;
	filepath = &argv[1];
	islastcmd = string_to_num(argv[5]);
	
	//printf("myhead.exe argc=%d ----- ",argc);
	//printf("argv[1]=%s,argv[2]=%s,argv[3]=%s,argv[4]=%s,argv[5]=%s\n",argv[1],argv[2],argv[3],argv[4],argv[5]);
	
	LeftMycatPipeFlag = find_left_mycat_pipe();
	LeftMyheadPipeFlag = find_left_myhead_pipe();
	int existence = LeftMycatPipeFlag || LeftMyheadPipeFlag;	//	Judge the existence of left pipe
	if(existence==1 && islastcmd==0){       //eg: myhead 3 file1.txt | mysort | mycat -n 
		//get data from pipe
		if(LeftMycatPipeFlag == 1)
			get_left_mycat_pipe();
		else if(LeftMyheadPipeFlag == 1)
			get_left_myhead_pipe();
		//create pipe, place data
		create_mysort_pipe();
	}
	else if(existence==0 && islastcmd==1){  //eg: mysort file1.txt
		//process data ,then print
		process_file_print();
	}
	else if(existence==1 && islastcmd==1){  //eg: myhead 3 file1.txt | mysort
		//get data from pipe,then print
		if(LeftMycatPipeFlag == 1)
			get_left_mycat_pipe();
		else if(LeftMyheadPipeFlag == 1)
			get_left_myhead_pipe();
	}
	else if(existence==0 && islastcmd==0){  //eg: mysort file1.txt | mycat -n 
		//process data ,put into buffer
		process_file_into_buffer();
		//create pipe, place data
		create_mysort_pipe();
	}

	/*
	int filenum = argc-1;
	char **filepath = &argv[1]; 

	vector<string> data;	//Store data
	string line;
	for(int i=0; i<filenum; i++){
		ifstream in(filepath[i]);
		while(getline(in,line))
			data.push_back(line);
	}
	sort(data.begin(),data.end());
	for(int i=0; i<data.size(); i++){
		cout<<data[i]<<endl;
	}
	*/

	//system("pause");
	return 0;
}
