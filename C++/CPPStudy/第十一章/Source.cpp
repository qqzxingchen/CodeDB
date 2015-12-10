
#include <iostream>
#include <stdlib.h>

using namespace std;

class Time{
	int hour, minute;
public:
	Time(int, int);
	inline int getHour() const { return hour; }
	inline int getMinute() const { return minute; }
};

Time::Time(int h, int m)
{
	hour = h;
	minute = m;
}

ostream& operator << (ostream& os, const Time & t)
{
	os << t.getHour() << t.getMinute() << endl;
	return os;
}


class Test{
	
public:
	Test(double s){ this->s = s; }
	double s;
	operator int();
};

void main()
{
	Test tt = 1.23;
	cout << tt.s << endl;;
	double m = tt;


	Time t(10, 200);
	cout << t;
	system("pause");
}

