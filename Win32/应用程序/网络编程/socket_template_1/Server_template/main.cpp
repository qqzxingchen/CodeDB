#include <stdio.h>
#include <winsock2.h>
//windowsock APIͷ�ļ�
 
#pragma comment(lib,"ws2_32.lib")
//windowsock APIͷ�ļ����ļ�
   
int main(){

	//����һ��WSAData���ݽṹ
	WSADATA wsadata;

	//WSAStartup��������Ӧ�ó�����winsock.dll�ĵ�һ������
	//��һ��������windows socket�汾��
	//�ڶ���������ָ��WSAData���ݵ�ָ��,�����洢ϵͳ���صĹ���winsocket������
	if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0){
		printf("WSAStartup failed:%d",GetLastError());
		return 0;
	}

	//����SOCKET�ṹ����socket���
	SOCKET m_socket;
	m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(m_socket==INVALID_SOCKET){
		printf("Error at socket():%d\n",GetLastError());
		WSACleanup();
		return 0;
	}
	
	//Ϊ��������һ�������ַ(IP+port),�Խ���һ������
	sockaddr_in service;				
	service.sin_family=AF_INET;			//Internet��ַ��
	service.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");	//��Ҫ�󶨵ı���IP��ַ
	service.sin_port=htons(27015);		//port

	//�󶨲�����Ƿ�ɹ���
	//��������Ҫִ����һ�����ͻ��˲���Ҫ
	if(bind(m_socket,(SOCKADDR*)&service,sizeof(service))==SOCKET_ERROR){
		printf("bind() failed.\n");
		closesocket(m_socket);
		return 0;
	}
	
	//����һ���������������ܿͻ��˵���������listen()����ʹserver��socket��������׶�
	if(listen(m_socket,1)==SOCKET_ERROR){
		printf("Error Listening on socket.\n");
	}

	printf("wainting for connect...\n");
	SOCKET AcceptSocket;

	//ʹ��ѭ���ȴ����򵥣����Ǻķ���Դ
	//���õ�ʹ���߳�
	while(1){
		AcceptSocket=SOCKET_ERROR;
		while(AcceptSocket==SOCKET_ERROR){

			//��accept���ȴ��������ӡ�һ�������ϣ��Ϳ���ͨ��AcceptSocket������ж�д
			AcceptSocket=accept(m_socket,NULL,NULL);
		}
		printf("Client Connected.\n");
		break;
	}
	return 0;
}