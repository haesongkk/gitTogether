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
	
	template <class T>
	void SafeDeleteArray(T& _t)
	{
		if (_t)
		{
			delete[] _t;
			_t = nullptr;
		}
	}
};

