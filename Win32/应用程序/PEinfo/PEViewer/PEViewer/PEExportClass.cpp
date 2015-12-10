
#include "stringTemplate.h"
#include "global.h"
#include "PEExportClass.h"

PEExportClass::PEExportClass(PEBase *peBase)
{
    this->peBase = peBase;
}


bool PEExportClass::writeToTempFile()
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

    writeStringToFile(tempFile,"var PEExportData = [");
    writeDataToFile(peFile,tempFile);
    writeStringToFile(tempFile,"];\n");

    CloseHandle(tempFile);
    CloseHandle(peFile);
    return true;
}


void PEExportClass::writeDataToFile( HANDLE peFile, HANDLE hFile )
{
    IMAGE_DATA_DIRECTORY dataDirectory;
    dataDirectory = peBase->optionHeader->getDataDirectory(0);

    if(dataDirectory.Size==0)
        return ;

    char temp[1024];
    char retu[1024];
    DWORD	byteNum;
    DWORD   addr;
    IMAGE_EXPORT_DIRECTORY exportDirectory;
    
    SetFilePointer(peFile,peBase->RvaToFoa(dataDirectory.VirtualAddress),NULL,FILE_BEGIN);
    ReadFile(peFile,&exportDirectory,sizeof(IMAGE_EXPORT_DIRECTORY),&byteNum,NULL);

    SetFilePointer(peFile,peBase->RvaToFoa(exportDirectory.Name),NULL,FILE_BEGIN);
    ReadFile(peFile,temp,MAX_PATH,&byteNum,NULL);

    writeStringToFile(hFile,"{name:'");
    writeStringToFile(hFile,temp);
    writeStringToFile(hFile,"',value:[");

    LPVOID ID = HeapAlloc(GetProcessHeap(),
                        HEAP_ZERO_MEMORY,
                        exportDirectory.NumberOfNames*sizeof(WORD));
    int IDIndex;

    SetFilePointer(peFile,peBase->RvaToFoa(exportDirectory.AddressOfNameOrdinals),NULL,FILE_BEGIN);
    for( DWORD i = 0 ; i < exportDirectory.NumberOfNames ; i++ )
        ReadFile(peFile,&((PWORD)ID)[i],sizeof(WORD),&byteNum,NULL);

    for ( int i = 0 ; i < (int)exportDirectory.NumberOfFunctions ; i++ ){

        IDIndex=-1;
        for( DWORD j = 0 ; j < exportDirectory.NumberOfNames ; j++ ){
            if( ((PWORD)ID)[j] == i ){
                IDIndex=j;
                break;
            }
        }
        
        st.setBaseStr( StringTemplate::exportTemplate );
        if(IDIndex==-1){
            st.exchange( "hint",(wsprintfA(temp,"%u",i),temp));            
            st.exchange( "name","无函数名，按序号导出");
        }else{
            st.exchange( "hint",( wsprintfA(temp,"%u",((PWORD)ID)[IDIndex]),temp ) );

            addr = peBase->RvaToFoa(exportDirectory.AddressOfNames);
            SetFilePointer(peFile,addr + IDIndex*sizeof(DWORD),NULL,FILE_BEGIN);		
            ReadFile(peFile,&addr,sizeof(DWORD),&byteNum,NULL);
            SetFilePointer(peFile,peBase->RvaToFoa(addr),NULL,FILE_BEGIN);					
            ReadFile(peFile,temp,MAX_PATH,&byteNum,NULL);

            st.exchange( "name",( wsprintfA(temp,"%s",temp),temp ));
        }

        SetFilePointer(peFile,peBase->RvaToFoa(exportDirectory.AddressOfFunctions)+i*sizeof(DWORD),NULL,FILE_BEGIN);		
        ReadFile(peFile,&addr,sizeof(DWORD),&byteNum,NULL);

        st.exchange("addrRVA",(wsprintfA(temp,"%d",addr),temp));

        st.getString( retu );

        if ( i != 0 )
            writeStringToFile(hFile,",");
        
        writeStringToFile( hFile,retu );
    }
    writeStringToFile(hFile,"]}");
}