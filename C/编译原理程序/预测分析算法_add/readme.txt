
typedef struct XC_grammar{
	int num;				
	char str[100][100];			//����ʽ

	char ter_symbol[100];		//�ս��

	char non_ter_symbol[100];	//���ս��

	char all_symbol[100];		//���з���

	char first_collection[100][100];

	char follow_collection[100][100];
}XC_grammar;


֧�ֵ��ַ���
	Сд��ĸ���ս��
	��д��ĸ�����ս��
				+ - * /
	$		����
	#		�����봮����
	|		���ķ������еĻ�

str�洢ÿ�����ʽ����str[0]���Ǵ洢 &->...  �ñ��ʽ
	&��ʾ�ķ��Ŀ�ʼ��  
