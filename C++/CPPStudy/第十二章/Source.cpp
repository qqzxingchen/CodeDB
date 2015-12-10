
#include <iostream>
#include <stdlib.h>

using namespace std;


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
class Test{
	int a, b;
public:
	Test(int a=10, int b=20);
	inline void print(){ cout << "a = " << a << " ; b = " << b << endl; }
	Test & operator=(const Test &);
	Test(const Test&);
};
Test::Test(int a, int b)
{
	this->a = a;
	this->b = b;
}
Test & Test::operator=(const Test & source){
	a = source.a;
	b = source.b;
	cout << " = copy" << endl;
	return *this;
}
Test::Test(const Test & source)
{
	a = source.a;
	b = source.b;
	cout << " copy copy" << endl;
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

class Temp{
private:
	int a, b;
	const int c;
		 
public:
	Temp();
	Temp(int a, int b);
};
Temp::Temp() :a(0), b(0), c(1)
{
	cout << c << endl;
}
Temp::Temp(int a,int b) : a(a),b(b),c(2)
{
	cout << c << endl;
}






void main()
{
	Test t1(10,20);
	Test t2 = t1;

	char * str = nullptr;
	char * s = "xingchen";
	
	char * k = new char[sizeof(char) * 1024 * 1024 * 1024];
	k[0] = '\0';
	cout << k << endl;

	Temp s1 = Temp(1, 2);
	Temp s2 = Temp();


	system("pause");
}

