#include "framework.h"
#include "Resource.h"

template <class T>
class Resource : public Sin
{
	map<string, shared_ptr<T>> res;
};
