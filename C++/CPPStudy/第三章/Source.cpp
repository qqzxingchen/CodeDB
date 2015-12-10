
#include <iostream>
#include <climits>
#include <stdlib.h>

using namespace std;


void func1()
{
	int n_int = INT_MAX;
	short n_short = SHRT_MAX;
	long n_long = LONG_MAX;
	long long n_llong = LLONG_MAX;

	cout << "int is " << sizeof(int) << " bytes." << endl;
	cout << "short is " << sizeof(short) << " bytes." << endl;
	cout << "long is " << sizeof(long) << " bytes." << endl;
	cout << "long long is " << sizeof(long long) << " bytes." << endl;

	cout << "Maximum values : " << endl;
	cout << "int : " << n_int << endl;
	cout << "short : " << n_short << endl;
	cout << "long : " << n_long << endl;
	cout << "long long : " << n_llong << endl;

}

void func2()
{
	int a = 100;
	cout << hex;
	cout << "a = (16进制)" << a << endl;
	cout << dec;
	cout << "a = (10进制)" << a << endl;
	cout << oct;
	cout << "a = (8进制)" << a << endl;
}

void func3()
{
	cout.setf(ios_base::fixed, ios_base::floatfield);
	float tree = 3;
	int guess(3.9832);
	int debt = 7.2E12;
	cout << "tree = " << tree << endl;
	cout << "guess = " << guess << endl;
	cout << "debt = " << debt << endl;

}


int main()
{
	//func1();
	//func2();
	func3();

	system("pause");
}

