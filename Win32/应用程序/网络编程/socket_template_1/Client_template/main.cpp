#include <stdio.h>
#include <winsock2.h>
//Windsock API的头文件

#pragma comment(lib,"ws2_32.lib")
//Windsock API的头文件库文件

int main(){
	WSADATA wsadata;
	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0){
		printf("WSAStartup failed:%d",GetLastError());
		return 0;
	}
	
	SOCKET m_socket;
	m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(m_socket==INVALID_SOCKET){
		printf("Error at socket():%d\n",GetLastError());
		WSACleanup();
		return 0;
	}
	
	sockaddr_in clientservice;
	clientservice.sin_family=AF_INET;
	clientservice.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
	clientservice.sin_port=htons(27015);

	if(connect(m_socket,(SOCKADDR*)&clientservice,sizeof(clientservice))==SOCKET_ERROR){
		printf("Failed to connect.\n");
		WSACleanup();
		return 0;
	}


	return 0;
}