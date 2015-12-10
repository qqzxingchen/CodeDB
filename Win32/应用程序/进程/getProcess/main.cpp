#include <stdio.h>
#include <windows.h>
#include <Tlhelp32.h>

int main(){
	HANDLE handle;
	HANDLE temp_handle;
	
	FILE *fp;
	fp=fopen("process.txt","w");

	handle=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,NULL);
	if(handle==INVALID_HANDLE_VALUE){
		printf("Wrong HANDLE!\n");
		return 0;
	}

	PROCESSENTRY32 process;
	MODULEENTRY32 temp_module;
	if(!Process32First(handle,&process)){
		printf("No Process now!\n");
		return 0;
	}

	while(1){
		fprintf(fp,"%-24s PID:%-5d\n",process.szExeFile,process.th32ProcessID);
		
		temp_handle=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,process.th32ProcessID);
		Module32First(temp_handle,&temp_module);
		while(1){
			fprintf(fp,"	%s\n",temp_module.szExePath);
			
			if(!Module32Next(temp_handle,&temp_module))
				break;
		}
		
		if(!Process32Next(handle,&process))
			break;
	}
	fclose(fp);
	return 0;
}