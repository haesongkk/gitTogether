#include "framework.h"
#include "Helper.h"

wstring Helper::ToWstring(const string& _str)
{
	wstring wstr(_str.begin(), _str.end());
	return wstr;
}

string Helper::ToString(const wstring& _wstr)
{
	string str(_wstr.begin(), _wstr.end());
	return str;
}