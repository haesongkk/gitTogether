#pragma once
#include <functional>
template <typename T>
using hsRes = map<string, T> m_resources;

template <typename T>
class hsResource
{
public:
	T GetResource(string _key)
	{
		assert(m_resources.find(_key) != m_resources.end())
		return m_resources[_key];
	}

private:
	map<string, T> m_resources;

};

template <typename T>
class hsLoader
{
public:
	void LoadFromFolder(string _folderPath, map<string, T>* _res)
	{

	}
	T Load(string _filePath) { }
};

template <>
int hsLoader<int>::Load(string _string)
{
	return int();
}
