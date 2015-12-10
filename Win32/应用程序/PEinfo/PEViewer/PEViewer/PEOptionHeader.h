#pragma once

#include <Windows.h>

class PEOptionHeader
{
public:
    static const char DllCharacterStr[][35];

    // PEOptionHeader32返回true，PEOptionHeader64返回false
    virtual bool isWin32PEFile() = 0;

    virtual int  getStructSize() = 0;

    // 获取dataDirectory目录的个数
    virtual int getDataDirectoryNum() = 0;

    // 获取第index个dataDirectory目录
    virtual IMAGE_DATA_DIRECTORY getDataDirectory(int index) = 0;

    virtual ULONGLONG getImageBase() = 0;

    // peFile要求是已经打开的file的句柄，而且当前位置是可以直接读取pe文件optionheader的位置
    virtual void readData(HANDLE peFile) = 0;

    // 要求是已经打开的file的句柄，且已经定位好了，函数将会直接向其中写入数据
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

