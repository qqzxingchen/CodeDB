#include "PERelocation.h"
#include "global.h"


PERelocation::PERelocation( PEBase *peBase )
{
    this->peBase = peBase;
}


bool PERelocation::writeToTempFile()
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

    writeStringToFile(tempFile,"var PERelocationData = [");
    writeDataToFile(peFile,tempFile);
    writeStringToFile(tempFile,"];\n");

    CloseHandle(tempFile);
    CloseHandle(peFile);
    return true;
}



void PERelocation::writeDataToFile( HANDLE peFile, HANDLE hFile )
{    
    IMAGE_DATA_DIRECTORY dataDirectory = peBase->optionHeader->getDataDirectory(5);
    
    if ( dataDirectory.VirtualAddress == 0 || dataDirectory.Size == 0 )
        return ;

    IMAGE_BASE_RELOCATION relocationTable;
    char temp[1024];
    DWORD byteNum;
    WORD originalAddrData;

    SetFilePointer(peFile,peBase->RvaToFoa(dataDirectory.VirtualAddress),NULL,FILE_BEGIN);

    bool firstSign = true;
    while(1){
        ReadFile(peFile,&relocationTable,sizeof(IMAGE_BASE_RELOCATION),&byteNum,NULL);

        if ( relocationTable.VirtualAddress == 0 )
            break;

        if ( firstSign ){
            firstSign = false;
        }else{
            writeSeparatorToFile(hFile);
        }

        writeStringToFile(hFile,"{BaseAddrRVA:");
        writeStringToFile(hFile,(wsprintfA(temp,"%u",relocationTable.VirtualAddress),temp));
        writeSeparatorToFile(hFile);
        writeStringToFile(hFile,"SizeOfBlock:");
        writeStringToFile(hFile,(wsprintfA(temp,"%u",relocationTable.SizeOfBlock),temp));
        writeSeparatorToFile(hFile);
        writeStringToFile(hFile,"DataArray:[");

        for( int i = 0 ; i < (relocationTable.SizeOfBlock-8)/2 ; i ++ ){
            ReadFile(peFile,&originalAddrData,sizeof(WORD),&byteNum,NULL);
            if ( i != 0 )
                writeSeparatorToFile(hFile);

            writeStringToFile(hFile, (wsprintfA(temp,"{value:%u,type:%u}",originalAddrData & 0xFFF,originalAddrData>>12),temp) );

//            switch ( originalAddrData>>12 )
//            {
//            case IMAGE_REL_BASED_ABSOLUTE:
//                writeStringToFile(hFile, (wsprintfA(temp,"{value:%u}",0xFFF),temp) );
//                break;
//            case IMAGE_REL_BASED_HIGHLOW:
//                writeStringToFile(hFile, (wsprintfA(temp,"{value:%u}",originalAddrData & 0xFFF),temp) );
//                break;
//            case IMAGE_REL_BASED_DIR64:
//                writeStringToFile(hFile, (wsprintfA(temp,"{value:%u,isDir64:true}",originalAddrData & 0xFFF),temp) );
//                break;
//            }            
        }
        writeStringToFile(hFile,"]}");
        
    }




}
