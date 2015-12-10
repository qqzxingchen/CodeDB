#include <stdio.h>
#include <winsock2.h>
//windowsock API头文件
 
#pragma comment(lib,"ws2_32.lib")
//windowsock API头文件库文件
   
int main(){

	//建立一个WSAData数据结构
	WSADATA wsadata;

	//WSAStartup函数连接应用程序与winsock.dll的第一个调用
	//第一个参数是windows socket版本号
	//第二个参数是指向WSAData数据的指针,用来存储系统传回的关于winsocket的资料
	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0){
		printf("WSAStartup failed:%d",GetLastError());
		return 0;
	}

	//定义SOCKET结构保存socket句柄
	SOCKET m_socket;
	m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(m_socket==INVALID_SOCKET){
		printf("Error at socket():%d\n",GetLastError());
		WSACleanup();
		return 0;
	}
	
	//为服务器绑定一个网络地址(IP+port),以接受一个连接
	sockaddr_in service;				
	service.sin_family=AF_INET;			//Internet地址族
	service.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");	//将要绑定的本地IP地址
	service.sin_port=htons(27015);		//port

	//绑定并检查是否成功绑定
	//服务器需要执行这一步，客户端不需要
	if(bind(m_socket,(SOCKADDR*)&service,sizeof(service))==SOCKET_ERROR){
		printf("bind() failed.\n");
		closesocket(m_socket);
		return 0;
	}
	
	//建立一个监听队列来接受客户端的连接请求，listen()函数使server的socket进入监听阶段
	if(listen(m_socket,1)==SOCKET_ERROR){
		printf("Error Listening on socket.\n");
	}

	printf("wainting for connect...\n");
	SOCKET AcceptSocket;

	//使用循环等待，简单，但是耗费资源
	//更好的使多线程
	while(1){
		AcceptSocket=SOCKET_ERROR;
		while(AcceptSocket==SOCKET_ERROR){

			//用accept来等待接受链接。一旦连接上，就可以通过AcceptSocket句柄进行读写
			AcceptSocket=accept(m_socket,NULL,NULL);
		}
		printf("Client Connected.\n");
		break;
	}
	return 0;
}