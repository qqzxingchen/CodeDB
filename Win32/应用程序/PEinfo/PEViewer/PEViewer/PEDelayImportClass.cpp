#include "PEDelayImportClass.h"
#include "global.h"

PEDelayImportClass::PEDelayImportClass( PEBase *peBase )
{
    this->peBase = peBase;
}


bool PEDelayImportClass::writeToTempFile(  )
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

    writeStringToFile(tempFile,"var PEDelayImportData = [");
    writeDataToFile(peFile,tempFile);
    writeStringToFile(tempFile,"];\n");

    CloseHandle(tempFile);
    CloseHandle(peFile);
    return true;
}


void PEDelayImportClass::writeDataToFile(HANDLE peFile, HANDLE hFile)
{
    if(peBase->optionHeader->getDataDirectory(13).Size == 0 )
        return ;

    DWORD importAddr = (DWORD)peBase->RvaToFoa(peBase->optionHeader->getDataDirectory(13).VirtualAddress);
    IMAGE_DELAY_IMPORT_DESCRIPTOR delayImport;
    IMAGE_THUNK_DATA thunkData;
    IMAGE_IMPORT_BY_NAME importName;
    DWORD byteNum;
    char temp[1024];
    int RVA_SIGN;

    bool totalFirstSign = true;
    for ( int i = 0 ; ; i ++ ){
        SetFilePointer(peFile,importAddr+i*sizeof(IMAGE_DELAY_IMPORT_DESCRIPTOR),NULL,FILE_BEGIN);
        ReadFile(peFile,&delayImport,sizeof(IMAGE_DELAY_IMPORT_DESCRIPTOR),&byteNum,NULL);

        if(delayImport.Name==0)
            break;

        if(delayImport.Attributes==1)       // 表示IMAGE_DELAY_IMPORT_DESCRIPTOR结构中的地址都是RVA，否则为VA
            RVA_SIGN=1;
        else
            RVA_SIGN=0;

        SetFilePointer(peFile,peBase->RvaToFoa( delayImport.Name - (1-RVA_SIGN)*peBase->optionHeader->getImageBase() ),NULL,FILE_BEGIN);
        ReadFile(peFile,temp,MAX_PATH,&byteNum,NULL);
        wsprintfA(&temp[byteNum],"\0");

        if ( !totalFirstSign )
            writeSeparatorToFile(hFile);
        else
            totalFirstSign = false;

        writeStringToFile(hFile,"{name:'");
        writeStringToFile(hFile,temp);
        writeStringToFile(hFile,"',value:[");

        ULONGLONG baseAddr = peBase->RvaToFoa( delayImport.DelayINT - (1-RVA_SIGN)*peBase->optionHeader->getImageBase() );

        bool funcFirstSign = true;
        for( int j = 0 ; ; j ++ )
        {
            SetFilePointer(peFile,(LONG)(baseAddr+j*sizeof(IMAGE_THUNK_DATA)),NULL,FILE_BEGIN);
            ReadFile(peFile,&thunkData,sizeof(IMAGE_THUNK_DATA),&byteNum,NULL);

            if(thunkData.u1.AddressOfData==0)
                break;

            if ( !funcFirstSign )
                writeSeparatorToFile(hFile);
            else
                funcFirstSign = false;

            if((thunkData.u1.AddressOfData & 0x80000000)!=0 )
            {
                st.setBaseStr( StringTemplate::importTemplate );
                st.exchange( "hint",( wsprintfA(temp,"%u",thunkData.u1.AddressOfData & 0xFFFF),temp ) );
                st.exchange( "name","无函数名，按序号导出");
                st.getString( temp );
                writeStringToFile( hFile,temp );
            }else{
                SetFilePointer(peFile,peBase->RvaToFoa(thunkData.u1.AddressOfData),NULL,FILE_BEGIN);
                ReadFile(peFile,&importName.Hint,sizeof(importName.Hint),&byteNum,NULL);

                ReadFile(peFile,temp,256,&byteNum,NULL);
                wsprintfA(&temp[byteNum],"\0");
                st.setBaseStr( StringTemplate::importTemplate );
                st.exchange( "name",temp );
                st.exchange( "hint",( wsprintfA(temp,"%u",importName.Hint),temp ) );
                st.getString( temp );
                writeStringToFile( hFile,temp );
            } 
        }		

        writeStringToFile(hFile,"]}");

    }
}
