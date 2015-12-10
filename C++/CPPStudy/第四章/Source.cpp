
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

struct MyStructTest
{
	unsigned int SN : 4;
	unsigned int : 4;
	bool goodIn : 1;
	bool goodTorgle : 1;
};



int main()
{
	string stemp1 = R"(asdkasmdpasmkdasd "" , \n)";
	string stemp2 = R"**++(asdkasmdpasmkdasd "" , \n)**++";

	cout << stemp1 << endl;
	cout << stemp2 << endl;
	cout << sizeof(MyStructTest) << endl;
	MyStructTest t{};
	for (int i = 0; i < 20; i++)
	{
		t.SN += 1;
		cout << t.SN << endl;
	}
	


	system("pause");
	return 0;
}