
#include <iostream>
#include <string>
#include <stdlib.h>

#include <new>

using namespace std;

struct chaff
{
	char dross[20];
	int slag;
};
char buffer1[50];
char buffer2[500];

void func()
{
	for (int i = 0; i < 50; i++)
		buffer1[i] = 1;
	for (int i = 0; i < 500; i++)
		buffer2[i] = 1;
	buffer1[49] = '\0';
	buffer2[499] = '\0';
	cout.setf(ios::hex);

	cout << "buffer1" << endl;
	for (int i = 0; i < 50; i++)
		cout << (int)buffer1[0];
	cout << endl << "buffer2" << endl;
	for (int i = 0; i < 500; i++)
		cout << (int)buffer2[0];

	chaff * p1, *p2;
	int * p3, *p4;
	p1 = new chaff;
	p3 = new int[20];
	p2 = new(buffer1)chaff;
	p4 = new(buffer2)int[20];
}


namespace Jack
{
	void p();
	class S;
}

namespace Jack{
	void p();
}
namespace Jack{
	void p()
	{
		
	}
}



void main()
{
	//func();
	Jack::p();


	system("pause");
}