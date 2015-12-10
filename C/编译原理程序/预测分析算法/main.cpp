#include "res.h"

char error_string[10]="err";

void control(char *input_string);
void display_grammar();

int main(){
	int i;
	
	char temp[100];
	char grammar_file[100];
	char input_string[100];
	
	printf("请输入你要使用的文法：\n");
	scanf("%s",grammar_file);
	printf("请输入你要分析的式子：\n");
	scanf("%s",input_string);


//	printf("%s",input_string);

// 	wsprintf(grammar_file,"grammar1.txt");
// 	wsprintf(input_string,"i*i+(i*i)+i#");

	readGrammar(grammar_file);
	insertSymbol();
	for(i=0;i<strlen(grammar.non_ter_symbol);i++)
		find_first_collection(grammar.non_ter_symbol[i],i);
	for(i=0;i<strlen(grammar.non_ter_symbol);i++)
		find_follow_collection(grammar.non_ter_symbol[i],i);
	find_str_first_collection();
	create_M(error_string);


	//display_grammar();


	control(input_string);


	return 0;
}

void control(char *input_string){
	int i,j,k;
	
	printf("\n\n");
	printf("输入串为：%s\n",input_string);

	if( input_string[strlen(input_string)-1]!='#'){
		wsprintf(input_string,"%s#",input_string);
	}	

	BOOL flag=TRUE;
	int read_input_string_num;
	char a;
	char X;
	int stack_point=0;
	char stack_string[1000];
	for(i=0;i<1000;i++)
		stack_string[i]='\0';

	stack_string[stack_point]='#';
	stack_point++;	
	stack_string[stack_point]=grammar.str[0][0];
	stack_point++;
	a=input_string[0];
	read_input_string_num=0;
	
	while(flag){
		stack_point--;
		X=stack_string[stack_point];
		stack_string[stack_point]='\0';
		if(check_char_in_string(X,grammar.ter_symbol) && X!='#'){
			if(X==a){
				read_input_string_num++;
				a=input_string[read_input_string_num];
				
			}else{
				printf("\n匹配错误,式子不是文法的产生式\n");
				return ;
			}
		}else if(X=='#'){
			if(X==a){
				flag=FALSE;
			}else{
				printf("\n匹配错误,式子不是文法的产生式\n");
				return ;
			}
		}else if(check_char_in_string(X,grammar.non_ter_symbol)){
			for(i=0;i<strlen(grammar.non_ter_symbol);i++)
				if(grammar.non_ter_symbol[i]==X)
					break;
			for(j=0;j<strlen(grammar.ter_symbol);j++)
				if(grammar.ter_symbol[j]==a)
					break;

			//printf("now is %c--%c\n",X,a);
			//printf("now is %d--%d\n",i,j);

			if(strcmp(grammar.M[i][j],error_string)==0){
				printf("\n匹配错误,式子不是文法的产生式\n");
				return ;
			}

			for(k=strlen(grammar.M[i][j])-1;k>=3;k--){
				if(grammar.M[i][j][k]!='$'){
					stack_string[stack_point]=grammar.M[i][j][k];
					stack_point++;
				}
			}
		}else{
			printf("\n匹配错误,式子不是文法的句子\n");
			return ;
		}
		printf("stack:%-10s  %15s    %-10s X==%c a==%c\n",
				stack_string,&input_string[read_input_string_num],
				check_char_in_string(X,grammar.non_ter_symbol)?grammar.M[i][j]:"NULL",
				X,a);
	}
	printf("\n输入串：匹配成功，该式是文法的句子\n");
}

void display_grammar(){
	int i,j,k;

	printf("\n\n");

	for(i=0;i<grammar.num;i++){
		printf("%s\n",grammar.str[i]);	
	}

	printf("\n%s--%d\n",grammar.non_ter_symbol,strlen(grammar.non_ter_symbol));
	printf("%s--%d\n",grammar.ter_symbol,strlen(grammar.ter_symbol));
	printf("%s==%d\n\n",grammar.all_symbol,strlen(grammar.all_symbol));

	for(i=0;i<strlen(grammar.non_ter_symbol);i++)
		printf("first[%c]  %s\n",grammar.non_ter_symbol[i],grammar.first_collection[i]);
	for(i=0;i<strlen(grammar.non_ter_symbol);i++)
		printf("follow[%c] %s\n",grammar.non_ter_symbol[i],grammar.follow_collection[i]);
	for(i=0;i<grammar.num;i++){	
		printf("str_first[   %-10s]:%s\n",grammar.str[i],grammar.str_first[i]);
	}
	
	printf("       ");
	for(i=0;i<strlen(grammar.ter_symbol);i++)
		printf("%-7c",grammar.ter_symbol[i]);
	printf("\n");
	
	for(i=0;i<strlen(grammar.non_ter_symbol);i++){
		printf("%c      ",grammar.non_ter_symbol[i]);
		for(j=0;j<strlen(grammar.ter_symbol);j++){
			printf("%-7s",grammar.M[i][j]);
		}
		printf("\n");
	}

	printf("\n\n");
}