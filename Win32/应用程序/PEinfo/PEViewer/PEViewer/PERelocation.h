#pragma once

#include "PEBaseClass.h"
#include "stringTemplate.h"

class PERelocation
{
private :
    PEBase * peBase;
    StringTemplate st;

    TCHAR ErrorString[256];

    void writeDataToFile( HANDLE peFile, HANDLE hFile );
public:
    PERelocation(PEBase *peBase);

    bool writeToTempFile();

    const TCHAR * GetTempFileName( ){
        return TEXT("temp\\relocation_data.txt");
    }
};

