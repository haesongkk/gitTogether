#pragma once
#include <functional>
template <typename T>
class hsResource
{
public:
	void LoadFromFolder(string _folderPath, function<T(string)> _createFunc)
	{

	}

	T GetResource(string _key)
	{
		assert(m_resources.find(_key) != m_resources.end())
		return m_resources[_key];
	}

private:
	map<string, T> m_resources;

};