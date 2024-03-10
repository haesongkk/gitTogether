#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class ObjectBase
{
protected:
	ObjectBase()
	{
		cout << "성공";
	}
	~ObjectBase() = default;

public:
	virtual shared_ptr<ObjectBase> Create() { return make_shared<ObjectBase>(); }

	friend class shared_ptr<ObjectBase>;

};

int main()
{
	ObjectBase* obj;
	shared_ptr<ObjectBase> test = obj->Create();
}

