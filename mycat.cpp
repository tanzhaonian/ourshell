/*mycat.cpp
Sample:  mycat.exe -n file1 file2
*/

#include<windows.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip> 
#include <vector>
using namespace std;

//Generic parameters
int LeftMyheadPipeFlag=0;	//Check the existence of left pipe
int LeftMysortPipeFlag=0;
int islastcmd = FALSE;
char buffer[1024];	//Pipe buffer
DWORD ReadNum;
DWORD WriteNum;

//Exclusive parameters
int filenum = 0;
char* argv1 = NULL;	//Judge the first parameter is "-n" or not
char **filepath = NULL;


int string_to_num(char *str) {
	int i=0,sum=0;
	while(str[i] != '\0') {
		sum=sum*10+str[i]-48;
		i++;
	}
	return sum;
}


char * num_to_string(int a){
	char t[20];
	sprintf(t,"%d",a);
	return t;
}

int find_left_myhead_pipe(){
	if(WaitNamedPipe("\\\\.\\Pipe\\myhead",NMPWAIT_WAIT_FOREVER)==FALSE){    //Waiting for myhead's pipe
		//printf("Mycat waits for myhead's pipe failed !!!\n");
		return FALSE;
	}
	else {
		//printf("Mycat waits for myhead's pipe successfully\n");
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
		printf("Mycat opens myhead's pipe failed !!!\n");
		return ;
	}
	else {		
		//printf("Mycat opens myhead's pipe successfully\n");
		if( ReadFile(hpipe,buffer,1024,&ReadNum,NULL)==0 )
			printf("Mycat reads myhead's pipe failed !!!\n");
		else {
			//printf("Mycat reads myhead's pipe successfully\n");
			buffer[ReadNum] = 0;

			//operation------------------------------------
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
			char temp[1024] = {0};
			int i;
			for(i=0; i<row-1; i++){
				strcat(temp,"  ");
				strcat(temp,num_to_string(i+1));
				strcat(temp,"  ");
				strcat(temp,data[i].c_str());
				strcat(temp,"\n");
			}
			strcat(temp,"  ");
			strcat(temp,num_to_string(i+1));
			strcat(temp,"  ");
			strcat(temp,data[i].c_str());

			strcpy(buffer,temp);
			printf("    After mycat:\n");
			printf("%s\n",buffer);

		}
	}//end big else
	CloseHandle(hpipe);
}

int find_left_mysort_pipe(){
	if(WaitNamedPipe("\\\\.\\Pipe\\mysort",NMPWAIT_WAIT_FOREVER)==FALSE){    //Waiting for mysort's pipe
		//printf("Mycat waits for mysort's pipe failed !!!\n");
		return FALSE;
	}
	else {
		//printf("Mycat waits for mysort's pipe successfully\n");
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
		printf("Mycat opens mysort's pipe failed !!!\n");
		return ;
	}
	else {	
		//printf("Mycat opens mysort's pipe successfully\n");
		if( ReadFile(hpipe,buffer,1024,&ReadNum,NULL)==0 )
			printf("Mycat reads mysort's pipe failed !!!\n");
		else {
			//printf("Mycat reads mysort's pipe successfully\n");
			buffer[ReadNum] = 0;

			//operation------------------------------------
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
			char temp[1024] = {0};
			int i;
			for(i=0; i<row-1; i++){
				strcat(temp,"  ");
				strcat(temp,num_to_string(i+1));
				strcat(temp,"  ");
				strcat(temp,data[i].c_str());
				strcat(temp,"\n");
			}
			strcat(temp,"  ");
			strcat(temp,num_to_string(i+1));
			strcat(temp,"  ");
			strcat(temp,data[i].c_str());

			strcpy(buffer,temp);
			printf("    After mycat:\n");
			printf("%s\n",buffer);

		}
	}//end big else
	CloseHandle(hpipe);
}

void create_mycat_pipe(){
	HANDLE hpipe = CreateNamedPipe(	//	Create pipe
		"\\\\.\\Pipe\\mycat",
		PIPE_ACCESS_DUPLEX,	// the pipe is bidirectional
		PIPE_TYPE_BYTE|PIPE_READMODE_BYTE,
		1,
		0,
		0,
		1000,
		NULL
		);
	if(hpipe == INVALID_HANDLE_VALUE)
		printf("Mycat creates mycat's pipe failed !!!\n");
	//else
	//	printf("Mycat creates mycat's pipe successfully\n");
	if(ConnectNamedPipe(hpipe,NULL) == FALSE){ //Waiting for connection
		CloseHandle(hpipe);
		printf("No one connects mycat's pipe !!!\n");
	}
	else {
		if( WriteFile(hpipe,buffer,strlen(buffer),&WriteNum,NULL)==FALSE )
			printf("Mycat writes into mycat's pipe failed !!!\n");
		//else
		//	printf("Mycat writes into mycat's pipe successfully\n");
	}
	if(DisconnectNamedPipe(hpipe) == FALSE)  //Disconnect
		printf("Mycat disconnects mycat's pipe failed !!!\n");
	//else
	//	printf("Mycat disconnects mycat's pipe successfully\n");
}

void process_file_print(){
	if(strcmp(argv1,"-n") == 0){  // Put row number then print
		vector<string> data;	//Store data by line
		string line;
		printf("    Before mycat:\n");
		for(int i=0; i<filenum; i++){
			ifstream in(filepath[i]);
			while(getline(in,line)){
				cout<<line<<endl;
				data.push_back(line);
			}
		}
		printf("    After mycat:\n");
		for(int i=0; i<data.size(); i++){
			cout<<setw(3)<<i+1<<"  "<<data[i]<<endl;
		}
	}//end if
	else {
		string line;
		for(int i=0; i<filenum; i++){
			ifstream in(filepath[i]);
			while(getline(in,line))
				cout<<line<<endl;
		}
	}
}

void process_file_into_buffer(){
	/*
	ifstream in(filepath);
	string line;
	for(int i=0; i<headnum; i++){
		getline(in,line);
		strcat(buffer,line.c_str());
	}
	*/
	if(strcmp(argv1,"-n") == 0){  // Put row number then print
		vector<string> data;	//Store data by line
		string line;
		printf("    Before mycat:\n");
		for(int i=0; i<filenum; i++){
			ifstream in(filepath[i]);
			while(getline(in,line)){
				cout<<line<<endl;
				data.push_back(line);
			}
		}
		int i;
		printf("    After mycat:\n");
		for(i=0; i<data.size()-1; i++){
			strcat(buffer,"  ");
			strcat(buffer,num_to_string(i+1));
			strcat(buffer,"  ");
			strcat(buffer,data[i].c_str());
			strcat(buffer,"\n");
		}
		strcat(buffer,"  ");
		strcat(buffer,num_to_string(i+1));
		strcat(buffer,"  ");
		strcat(buffer,data[i].c_str());
		printf("%s\n",buffer);
	}//end if
}

int main(int argc,char* argv[]){
	if(strcmp(argv[1],"-n") != 0){  //Just show the content of the file,such as "mycat file1.txt file2.txt"
		filenum = argc-2;
		filepath = &argv[1];
		argv1 = argv[1];
		process_file_print();
	}
	else{		//Put row number for each row of the file,such as "mycat -n file1.txt"
		filenum = argc-2;
		filepath = &argv[2];
		argv1 = argv[1];
		islastcmd = string_to_num(argv[5]);

		//printf("mycat.exe argc=%d ------ ",argc);
		//printf("argv[1]=%s,argv[2]=%s,argv[3]=%s,argv[4]=%s,argv[5]=%s\n",argv[1],argv[2],argv[3],argv[4],argv[5]);

		LeftMyheadPipeFlag = find_left_myhead_pipe();
		LeftMysortPipeFlag = find_left_mysort_pipe();
		int existence = LeftMyheadPipeFlag || LeftMysortPipeFlag;	//	Judge the existence of left pipe
		if(existence==1 && islastcmd==0){       //eg: myhead 3 file1.txt | mycat -n | mysort
			//get data from pipe
			if(LeftMyheadPipeFlag == 1)
				get_left_myhead_pipe();
			else if(LeftMysortPipeFlag == 1)
				get_left_mysort_pipe();
			//create pipe, place data
			create_mycat_pipe();
		}
		else if(existence==0 && islastcmd==1){  //eg: mycat -n file1.txt
			//process data ,then print
			process_file_print();
		}
		else if(existence==1 && islastcmd==1){  //eg: myhead 3 file1.txt | mycat -n
			//get data from pipe,then print
			if(LeftMyheadPipeFlag == 1)
				get_left_myhead_pipe();
			else if(LeftMysortPipeFlag == 1)
				get_left_mysort_pipe();
		}
		else if(existence==0 && islastcmd==0){  //eg: mycat -n file1.txt | myhead 3
			//process data ,put into buffer
			process_file_into_buffer();
			//create pipe, place data
			create_mycat_pipe();
		}
	}
	/*
	if(strcmp(argv[1],"-n") == 0){  // Put row number then print
			int filenum = argc-2;
			char **filepath = &argv[2];

			vector<string> data;	//Store data by line
			string line;
			for(int i=0; i<filenum; i++){
				ifstream in(filepath[i]);
				while(getline(in,line))
					data.push_back(line);
			}
			for(int i=0; i<data.size(); i++){
				cout<<setw(4)<<i+1<<"  "<<data[i]<<endl;
			}
	}//end if
	*/
	//system("pause");
	return 0;
}