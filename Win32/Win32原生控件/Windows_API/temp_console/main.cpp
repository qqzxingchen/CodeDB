#include <stdio.h>
#include <windows.h>


int main(){
	HANDLE han;
	WIN32_FIND_DATA fd;

	han=FindFirstFile("c:\\*",&fd);
	while(FindNextFile(han,&fd)){
		printf("%s\n",fd.cFileName);
	}
	return 0;
}