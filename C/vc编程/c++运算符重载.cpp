#include<iostream>
using namespace std;
class point
{
public:
	point(int a)
	{
	   m_left=a;
	}
	~point()
	{
	
	}
    point operator + (point& a) 
	{
	   this->m_left=this->m_left + a.m_left;
	   cout<<"+"<<"   ";
	   showit();
	   return *this;
	}
	point operator -(point& a)
	{
	   this->m_left=this->m_left - a.m_left;
	   cout<<"-"<<"   ";
	   showit();
	   return *this;
	}
	void showit()
	{
	   cout<<m_left<<endl;
	}

	int m_left;
};
int main()
{ 
	point obj(4);
    obj=(obj-obj)+(obj+obj);
	obj.showit();
    return 0;
}