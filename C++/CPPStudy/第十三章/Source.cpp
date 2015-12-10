
#include <stdlib.h>
#include <iostream>

using namespace std;

class Parent{
public:
	int a;
	Parent(int a = 10){ this->a = a; }
};
class Child : public Parent{
public:
	int a;
	Child(int a = 20) :Parent(5), a(a){}
};

class B
{
	int a = 0;
public:
	virtual void p(){ cout << a << endl; }
};
class D : public B{
	int a = 1;
public:
	virtual void p(){ cout << a << endl; }
};

void main(){

	B b;
	D d;
	B & bb = d;
	bb.p();

	Child c = Child(30);
	cout << c.a << endl;

	Parent * p = &c;
	cout << p->a << endl;

	system("pause");
}





