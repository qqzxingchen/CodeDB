
#include <iostream>
#include <string>
#include <stdlib.h>

class Test
{
public:
	Test(int i );
	~Test();
	void print();

};

Test::Test(int i)
{
	std::cout << "new" << i << std::endl;
}
Test::~Test()
{
	std::cout << "delete" << std::endl;
}


void Test::print()
{
	static int count = 0;
	std::cerr << count << std::endl;
	count++;
}



void main()
{
	Test t1(1);
	Test t2 = Test(2);
	for (int i = 0; i < 10; i++){
		t1.print();
		t2.print();
	}


	system("pause");
}