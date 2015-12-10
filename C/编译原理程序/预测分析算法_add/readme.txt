
typedef struct XC_grammar{
	int num;				
	char str[100][100];			//产生式

	char ter_symbol[100];		//终结符

	char non_ter_symbol[100];	//非终结符

	char all_symbol[100];		//所有符号

	char first_collection[100][100];

	char follow_collection[100][100];
}XC_grammar;


支持的字符：
	小写字母：终结符
	大写字母：非终结符
				+ - * /
	$		：空
	#		：输入串结束
	|		：文法规则中的或

str存储每个表达式。且str[0]总是存储 &->...  该表达式
	&表示文法的开始符  
