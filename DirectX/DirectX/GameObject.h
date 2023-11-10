#pragma once
class Component;
class GameObject
{
	vector<shared_ptr<Component>> m_spComponents;

public:
	// 생성자
	GameObject();

public:
	// 소멸자
	~GameObject();

public:
	// 업데이트
	void Run();
	
public:
	template<class Comp>
	shared_ptr<Comp> AddComponent()
	{
		shared_ptr<Comp> spComponent = make_shared<Comp>(GetSharedPtr());
		return spComponent;
	}

	template<class Comp>
	shared_ptr<Comp> GetComponent()
	{
		for (auto comp : m_spComponents)
			if(auto castComp = dynamic_pointer_cast<Comp>(comp))
				return castComp;
		return nullptr;
	}


	shared_ptr<GameObject> GetSharedPtr()
	{
		return shared_ptr<GameObject>(this);
	}
};

