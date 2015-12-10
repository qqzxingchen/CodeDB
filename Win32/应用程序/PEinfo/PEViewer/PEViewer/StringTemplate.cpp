#include <string.h>
#include "StringTemplate.h"

const char StringTemplate::baseTemplate[] = "{name:'<<name>>',value:<<value>>,length:<<length>>,desc:'<<desc>>'}";
const char StringTemplate::importTemplate[] = "{hint:<<hint>>,name:'<<name>>'}";
const char StringTemplate::exportTemplate[] = "{hint:<<hint>>,name:'<<name>>',address:<<addrRVA>>}";


void StringTemplate::setBaseStr( const char baseStr[] ){
	wsprintfA( this->baseStr,"%s",baseStr );
}

void StringTemplate::exchange( char * name,char * value ){
	char temp[128];
	wsprintfA( temp,"<<%s>>",name );
	int	tempLength = strlen(temp);

	int index = this->stringContains(this->baseStr,temp);
	if ( index == -1 )
		return;

	char right[1024];
	memset( right,0,1024 );

	strncpy( right,
						&this->baseStr[index+tempLength],
						strlen(this->baseStr)-index-tempLength );
	wsprintfA( &this->baseStr[index],"%s%s",value,right );
}

int StringTemplate::stringContains( char * source,char * checkStr )
{
	for ( int i = 0 ; source[i] ; i ++ ){
		if ( strncmp( &source[i],checkStr,strlen(checkStr) ) == 0 ){
			return i;
		}
	}
	return -1;
}

int StringTemplate::getLength()
{
	return strlen( this->baseStr );
}

void StringTemplate::getString( char * temp )
{
	wsprintfA( temp,"%s",this->baseStr );
}


