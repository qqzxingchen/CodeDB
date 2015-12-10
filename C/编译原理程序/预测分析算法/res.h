#include <stdio.h>
#include <windows.h>



typedef struct XC_grammar{
	int num;				
	char str[100][100];			//产生式

	char str_first[100][100];	//每个产生式右部的first集

	char ter_symbol[100];		//终结符

	char non_ter_symbol[100];	//非终结符

	char all_symbol[100];		//所有符号

	char first_collection[100][100];	//非终结符的first集

	char follow_collection[100][100];	//非终结符的follow集

	char M[100][100][100];
}XC_grammar;

XC_grammar grammar;

int sign_first[100];
int sign_follow[100];


int readGrammar(char *inputgrammar){
	FILE *inputFp_inputgrammar;
	char inputchar;
	char tempchar;
	int sign;
	int i,j,k;
	
	for(i=0;i<100;i++){
		for(j=0;j<100;j++){
			grammar.str[i][j]='\0';
			grammar.str_first[i][j]='\0';
			grammar.first_collection[i][j]='\0';
			grammar.follow_collection[i][j]='\0';
		}
	}
	
	for(i=0;i<100;i++){
		sign_first[i]=0;
		sign_follow[i]=0;
	}
	for(i=0;i<100;i++)
		for(j=0;j<100;j++)
			for(k=0;k<100;k++)
				grammar.M[i][j][k]='\0';




	inputFp_inputgrammar=fopen(inputgrammar,"r");

	sign=0;
	grammar.num=1;
	i=0;
	while(fscanf(inputFp_inputgrammar,"%c",&inputchar)!=EOF){
		if(inputchar==' ')
			continue;
		if(i==0)
			tempchar=inputchar;
		if(sign==1){
			grammar.num++;
			wsprintf(grammar.str[grammar.num],"%c->",tempchar);
			i=3;
			sign=0;
		}

		if(	(inputchar>='A' && inputchar<='Z')||
			(inputchar>='a' && inputchar<='z')||
			inputchar=='+' || inputchar=='-'  ||
			inputchar=='*' || inputchar=='/'  ||
			inputchar=='(' || inputchar==')'  ||
			inputchar=='>' || inputchar=='$' )
		{
			grammar.str[grammar.num][i]=inputchar;
			i++;
			continue;
		}else if(inputchar==';'){
			grammar.num++;
			i=0;
			continue;
		}else if( inputchar=='|' ){
			sign=1;
			continue;
		}
	}
	wsprintf(grammar.str[0],"&->%c",grammar.str[1][0]);
		
	fclose(inputFp_inputgrammar);
	return 0;
}

void insert_char_without_repeat(char insertchar,char *temp){
	int i=0;
	int sign=0;
	while(temp[i]!='\0'){
		if(temp[i]==insertchar)
			sign=1;
		i++;
	}
	if(sign==0){
		temp[strlen(temp)]=insertchar;
		temp[strlen(temp)+1]='\0';
	}
}


void insertSymbol(){
	int i,j;

	for(i=0;i<grammar.num;i++){
		insert_char_without_repeat(grammar.str[i][0],grammar.non_ter_symbol);
	}

	for(i=0;i<grammar.num;i++){
		j=3;
		while(grammar.str[i][j]!='\0'){
			if(  (grammar.str[i][j]>='a' && grammar.str[i][j]<='z') ||
				  grammar.str[i][j]=='+' || grammar.str[i][j]=='-'  ||
				  grammar.str[i][j]=='*' || grammar.str[i][j]=='/'  ||
				  grammar.str[i][j]=='(' || grammar.str[i][j]==')'  ||
				  grammar.str[i][j]=='i' || grammar.str[i][j]=='$' )
			{
				insert_char_without_repeat(grammar.str[i][j],grammar.ter_symbol);
			}

			j++;
		}
	}
	wsprintf(grammar.all_symbol,"%s",grammar.non_ter_symbol);
	wsprintf(&grammar.all_symbol[strlen(grammar.all_symbol)],"%s",grammar.ter_symbol);

	insert_char_without_repeat('#',grammar.ter_symbol);
}

int check_char_in_string(char tempchar,char * tempstring){
	int i;
	for(i=0;i<strlen(tempstring);i++){
		if(tempchar==tempstring[i])
			return 1;
	}
	return 0;
}



void find_first_collection(char non_ter_char,int n){
	int i,j,k;
	int sign=0;

	for(i=0;i<grammar.num;i++){
		if(grammar.str[i][0]==non_ter_char){
			if(check_char_in_string(grammar.str[i][3],grammar.ter_symbol) || grammar.str[i][3]=='$'){
				insert_char_without_repeat(grammar.str[i][3],grammar.first_collection[n]);
			}else if(check_char_in_string(grammar.str[i][3],grammar.non_ter_symbol)){
				if(grammar.str[i][3]==non_ter_char)
					continue;
				for(k=0;;k++){
					if(grammar.non_ter_symbol[k]==grammar.str[i][3])
						break;
				}
				if(sign_first[k]==0){
					find_first_collection(grammar.non_ter_symbol[k],k);
				}
				for(j=0;j<strlen(grammar.first_collection[k]);j++){
					if(grammar.first_collection[k][j]!='$')
						insert_char_without_repeat(grammar.first_collection[k][j],grammar.first_collection[n]);
				}
				if(check_char_in_string('$',grammar.first_collection[k])){
					j=4;
					sign=0;
					while(check_char_in_string(grammar.str[i][j],grammar.non_ter_symbol)){
						for(k=0;;k++){
							if(grammar.non_ter_symbol[k]==grammar.str[i][j])
								break;
						}
						if(sign_first[k]==0){
							find_first_collection(grammar.non_ter_symbol[k],k);
						}
						if(!check_char_in_string('$',grammar.first_collection[k])){
							sign=1;
							break;
						}
						j++;
					}
				}
			}
		}
	}
	sign_first[n]=1;
}

void find_str_first_collection(){
	int i,j,k,m,n;
	int sign;

	for(i=0;i<grammar.num;i++){
		j=3;
		sign=0;
		while(check_char_in_string(grammar.str[i][j],grammar.non_ter_symbol)){
			for(k=0;;k++)
				if(grammar.non_ter_symbol[k]==grammar.str[i][j])
					break;
			for(m=0;m<strlen(grammar.first_collection[k]);m++)
				if(grammar.first_collection[k][m]!='$')
					insert_char_without_repeat(grammar.first_collection[k][m],grammar.str_first[i]);
			if(!check_char_in_string('$',grammar.first_collection[k])){
				sign=1;
				break;
			}
			j++;
		}
		if(sign==0){
			if(grammar.str[i][j]=='\0')
				insert_char_without_repeat('$',grammar.str_first[i]);
			else if(check_char_in_string(grammar.str[i][j],grammar.ter_symbol))
				insert_char_without_repeat(grammar.str[i][j],grammar.str_first[i]);
		}
	}
}




void find_follow_collection(char non_ter_char,int n){
	int i,j,k,m,p;
	int sign;
// 	for(i=0;;i++){
// 		if(grammar.str[0][3]==grammar.non_ter_symbol[i])
// 			break;
// 	}
	insert_char_without_repeat('#',grammar.follow_collection[0]);

	

	for(i=0;i<grammar.num;i++){
		for(j=3;j<strlen(grammar.str[i]);j++){
			if(grammar.str[i][j]==non_ter_char){
				if(grammar.str[i][j+1]=='\0'){	

					if(grammar.str[i][j]==grammar.str[i][0])
						continue;

					for(k=0;;k++){
						if(grammar.str[i][0]==grammar.non_ter_symbol[k])
							break;
					}
					if(sign_follow[k]==0){
						find_follow_collection(grammar.non_ter_symbol[k],k);
					}
					for(m=0;m<strlen(grammar.follow_collection[k]);m++)
						insert_char_without_repeat(grammar.follow_collection[k][m],grammar.follow_collection[n]);
				}else{
					if(check_char_in_string(grammar.str[i][j+1],grammar.non_ter_symbol)){
						for(k=0;;k++){
							if(grammar.str[i][j+1]==grammar.non_ter_symbol[k])
								break;
						}
						if(sign_follow[k]==0){
							find_follow_collection(grammar.non_ter_symbol[k],k);
						}
						for(m=0;m<strlen(grammar.follow_collection[k]);m++)
							if(grammar.first_collection[k][m]!='$')
								insert_char_without_repeat(grammar.first_collection[k][m],grammar.follow_collection[n]);
					}else if(check_char_in_string(grammar.str[i][j+1],grammar.ter_symbol)){
						insert_char_without_repeat(grammar.str[i][j+1],grammar.follow_collection[n]);
					}

					k=j+1;
					sign=0;
					while(check_char_in_string(grammar.str[i][k],grammar.non_ter_symbol)){
	
						for(m=0;;m++)
							if(grammar.non_ter_symbol[m]==grammar.str[i][k])
								break;
						if(check_char_in_string('$',grammar.first_collection[m])){
							k++;
							continue;
						}
						for(p=0;p<strlen(grammar.first_collection[m]);p++){
							insert_char_without_repeat(grammar.first_collection[m][p],grammar.follow_collection[n]);
						}
						sign=1;
						break;
					}
					if(sign==0 && check_char_in_string(grammar.str[i][k],grammar.ter_symbol)){
						insert_char_without_repeat(grammar.str[i][k],grammar.follow_collection[n]);
					}else if(sign==0 && grammar.str[i][k]=='\0'){
						
						if(grammar.str[i][j]==grammar.str[i][0]){
							continue;
						}
							
						for(k=0;;k++){
							if(grammar.str[i][0]==grammar.non_ter_symbol[k])
								break;
						}
						if(sign_follow[k]==0){
							find_follow_collection(grammar.non_ter_symbol[k],k);
						}
						for(m=0;m<strlen(grammar.follow_collection[k]);m++)
							insert_char_without_repeat(grammar.follow_collection[k][m],grammar.follow_collection[n]);
			
					}
				}
			}
		}
	}
	sign_follow[n]=1;
}


void create_M(char *error_string){
	int i,j,k,m;

	for(i=0;i<grammar.num;i++){
		for(k=0;;k++)
			if(grammar.non_ter_symbol[k]==grammar.str[i][0])
					break;

		for(j=0;j<strlen(grammar.str_first[i]);j++){
			for(m=0;;m++)
				if(grammar.ter_symbol[m]==grammar.str_first[i][j])
					break;
			wsprintf(grammar.M[k][m],"%s",grammar.str[i]);
		}
		if(check_char_in_string('$',grammar.str_first[i])){
			for(j=0;j<strlen(grammar.follow_collection[k]);j++){
				for(m=0;;m++)
					if(grammar.follow_collection[k][j]==grammar.ter_symbol[m])
						break;
				wsprintf(grammar.M[k][m],"%s",grammar.str[i]);
			}
		}
	}
	for(i=0;i<strlen(grammar.non_ter_symbol);i++){
		for(j=0;j<strlen(grammar.ter_symbol);j++){
			if(grammar.M[i][j][0]=='\0')
				wsprintf(grammar.M[i][j],error_string);
		}
	}
}
