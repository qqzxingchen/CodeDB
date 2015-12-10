#pragma once

#include <Windows.h>

class PEOptionHeader
{
public:
    static const char DllCharacterStr[][35];

    // PEOptionHeader32����true��PEOptionHeader64����false
    virtual bool isWin32PEFile() = 0;

    virtual int  getStructSize() = 0;

    // ��ȡdataDirectoryĿ¼�ĸ���
    virtual int getDataDirectoryNum() = 0;

    // ��ȡ��index��dataDirectoryĿ¼
    virtual IMAGE_DATA_DIRECTORY getDataDirectory(int index) = 0;

    virtual ULONGLONG getImageBase() = 0;

    // peFileҪ�����Ѿ��򿪵�file�ľ�������ҵ�ǰλ���ǿ���ֱ�Ӷ�ȡpe�ļ�optionheader��λ��
    virtual void readData(HANDLE peFile) = 0;

    // Ҫ�����Ѿ��򿪵�file�ľ�������Ѿ���λ���ˣ���������ֱ��������д������
    virtual void writeDataToFile(  HANDLE tempFile  ) = 0;
};

class PEOptionHeader32 : public PEOptionHeader
{
private :
    IMAGE_OPTIONAL_HEADER32 optionHeader;
public : 
    bool isWin32PEFile(){
        return true;
    }
    int  getStructSize(){
        return sizeof( IMAGE_OPTIONAL_HEADER32 );
    }
    int getDataDirectoryNum(){
        return optionHeader.NumberOfRvaAndSizes;
    }
    IMAGE_DATA_DIRECTORY getDataDirectory(int index){
        return optionHeader.DataDirectory[index];
    }
    ULONGLONG getImageBase(){
        return optionHeader.ImageBase;
    }
    void readData( HANDLE peFile ){
        DWORD byteNum;
        ReadFile(peFile,&this->optionHeader,sizeof(IMAGE_OPTIONAL_HEADER32),&byteNum,NULL);
    }

    void writeDataToFile( HANDLE tempFile );
};


class PEOptionHeader64 : public PEOptionHeader
{
private:
    IMAGE_OPTIONAL_HEADER64 optionHeader;
public :
    bool isWin32PEFile(){
        return false;
    }
    int  getStructSize(){
        return sizeof( IMAGE_OPTIONAL_HEADER64 );
    }
    int getDataDirectoryNum(){
        return optionHeader.NumberOfRvaAndSizes;
    }
    IMAGE_DATA_DIRECTORY getDataDirectory(int index){
        return optionHeader.DataDirectory[index];
    }
    ULONGLONG getImageBase(){
        return optionHeader.ImageBase;
    }
    void readData( HANDLE peFile ){
        DWORD byteNum;
        ReadFile(peFile,&this->optionHeader,sizeof(IMAGE_OPTIONAL_HEADER64),&byteNum,NULL);
    }

    void writeDataToFile(  HANDLE tempFile );
};

