#pragma once
namespace Helper
{
	template <class T>
	void SafeRelease(T& _t)
	{
		if (_t)
		{
			_t->Release();
			_t = nullptr;
		}
	}

	template <class T>
	void SafeDelete(T& _t)
	{
		if (_t)
		{
			delete _t;
			_t = nullptr;
		}
	}
	
	wstring ToWstring(const string& _str);
	string ToString(const wstring& _wstr);
};