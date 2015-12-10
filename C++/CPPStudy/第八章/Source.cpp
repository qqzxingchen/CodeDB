
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;


template<class T>
T Add(T a, T b)
{ 
	return a + b; 
}


template<class T>
T AddExt(T a, T b)
{
	return a + b;
}
template int AddExt<int>(int a, int b)
{
	return a + b + 1;
}



int main()
{

	int m = 6;
	double x = 10.2;
	cout << Add(x, m) << endl;          // 显示具体化的使用
	cout << Add<double>(x, m) << endl;          // 显示具体化的使用

	system("pause");
	return 0;
}