#include <winsock2.h>
#include <stdio.h>
#include <time.h>
#pragma comment(lib,"ws2_32.lib")

clock_t start,end;
float costtime;

void usage(void){
	printf("\tusage:tcpscan RemoteIP StartPort-Endport\n");
	printf("\tExample:tcpscan 127.0.0.1 100-500\n");
}

int main(int argc,char * argv[]){
	char *host;
	int startport,endport;
	char *p;
	if(argc!=3){
		usage();
		return 0;
	}

	p=argv[2];
	if(strstr(argv[2],"-")){
		startport=atoi(argv[2]);
		for(;*p;){
			if(*(p++)=='-')
				break;
		}
		endport=atoi(p);

		if(startport<1 || endport>65535){
			printf("Port Error!\n");
			return 0;
		}
	}

	host=argv[1];
	WSADATA ws;
	SOCKET s;
	int i;
	struct sockaddr_in addr;
	int result;
	long lresult;
	lresult=WSAStartup(MAKEWORD(2,2),&ws);
	addr.sin_family=AF_INET;
	addr.sin_addr.S_un.S_addr=inet_addr(host);
	start=clock();
	u_long iMode=0;


	for(i=startport;i<=endport;i++){
		s=socket(AF_INET,SOCK_STREAM,0);

		addr.sin_port=htons(i);
		if(s==INVALID_SOCKET)
			break;
		result=connect(s,(struct sockaddr*)&addr,sizeof(addr));
		if(result==0){
			printf("%s %d open\n",host,i);
			closesocket(s);
		}
	}
	end=clock();
	costtime=(float)(end-start);

	printf("Cost time:%f second",costtime);
	WSACleanup();
	getchar();
	return 0;
}
	