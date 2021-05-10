#pragma once
#include <iostream>
using namespace std;

class CGameException : public exception
{
public:
	CGameException(string err)
	{
		cout << err << endl;
	}
};

