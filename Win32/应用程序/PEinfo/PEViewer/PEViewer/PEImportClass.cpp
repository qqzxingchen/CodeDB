#include "PEImportClass.h"
#include "PEBaseClass.h"
#include "global.h"

PEImportClass::PEImportClass(PEBase * peBase)
{
	this->peBase = peBase;
}

bool PEImportClass::writeToTempFile()
{
	CreateDirectory( TEXT("temp"),NULL );

	HANDLE peFile = CreateFile(peBase->getPEFilePath(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if ( peFile == INVALID_HANDLE_VALUE ){
		int a = GetLastError();
		wsprintf(ErrorString,TEXT("打开PE文件失败"));
		return false;
	}

	HANDLE tempFile;
	tempFile = CreateFile( this->GetTempFileName(),
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL );
	if ( tempFile == INVALID_HANDLE_VALUE ){
		int a = GetLastError();
		wsprintf(ErrorString,TEXT("创建缓存文件失败"));
		return false;
	}

	writeStringToFile(tempFile,"var PEImportData = [");
	writeDataToFile(peFile,tempFile);
	writeStringToFile(tempFile,"];\n");

	CloseHandle(tempFile);
    CloseHandle(peFile);
	return true;
}


void PEImportClass::writeDataToFile( HANDLE peFile, HANDLE hFile )
{
    if(peBase->optionHeader->getDataDirectory(1).Size==0)
		return ;

	bool funcFirstSign;
	bool totalFirstSign;

	DWORD byteNum;
	DWORD rva,foa;
	char temp[1024];
	char retu[1024];
	IMAGE_IMPORT_DESCRIPTOR import;
	IMAGE_THUNK_DATA		thunk;
	IMAGE_IMPORT_BY_NAME	importByName;
    DWORD importAddr=(DWORD)peBase->RvaToFoa((ULONGLONG)(peBase->optionHeader->getDataDirectory(1).VirtualAddress));

	totalFirstSign = true;

	for ( int i = 0 ; ; i ++ )
	{
		memset( &import,0,sizeof(IMAGE_IMPORT_DESCRIPTOR) );

		SetFilePointer(peFile,importAddr+i*sizeof(IMAGE_IMPORT_DESCRIPTOR),NULL,FILE_BEGIN);
		ReadFile(peFile,&import,sizeof(IMAGE_IMPORT_DESCRIPTOR),&byteNum,NULL);

		if((int)import.Name==0)
			break;

		SetFilePointer(peFile,peBase->RvaToFoa((int)import.Name),NULL,FILE_BEGIN);
		ReadFile(peFile,temp,256,&byteNum,NULL);

		if ( !totalFirstSign )
			writeSeparatorToFile(hFile);
		else
			totalFirstSign = false;
		
		writeStringToFile(hFile,"{name:'");
		writeStringToFile(hFile,temp);
		writeStringToFile(hFile,"',value:[");
		funcFirstSign = true;

		rva = ( import.OriginalFirstThunk != 0 ) ? import.OriginalFirstThunk : import.FirstThunk;
		foa = peBase->RvaToFoa(rva);
		
		for( int j = 0 ; ; j ++ )
		{
			SetFilePointer(peFile,foa+j*sizeof(IMAGE_THUNK_DATA),NULL,FILE_BEGIN);
			ReadFile(peFile,&thunk,sizeof(IMAGE_THUNK_DATA),&byteNum,NULL);

			if(thunk.u1.AddressOfData==0)
				break;
			
			if ( !funcFirstSign )
				writeSeparatorToFile(hFile);
			else
				funcFirstSign = false;
			

			if(((DWORD)thunk.u1.AddressOfData & 0x80000000)!=0 )
			{
				st.setBaseStr( StringTemplate::importTemplate );
				st.exchange( "hint",( wsprintfA(temp,"%u",thunk.u1.AddressOfData & 0xFFFF),temp ) );
				st.exchange( "name","无函数名，按序号导出");
				st.getString( retu );
				writeStringToFile( hFile,retu );
			}else{
				SetFilePointer(peFile,peBase->RvaToFoa(thunk.u1.AddressOfData),NULL,FILE_BEGIN);
				ReadFile(peFile,&importByName,sizeof(WORD),&byteNum,NULL);

				st.setBaseStr( StringTemplate::importTemplate );
				ReadFile(peFile,temp,256,&byteNum,NULL);
				st.exchange( "name",temp );
				st.exchange( "hint",( wsprintfA(temp,"%u",importByName.Hint),temp ) );
				st.getString( retu );
				writeStringToFile( hFile,retu );
			}
		}		
		writeStringToFile(hFile,"]}");
	}
}

