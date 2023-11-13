#pragma once
class Manager;
class Resource;
class Process
{
	spvector<Manager> m_spManagers;
	spvector<Resource> m_spResources;

public:
	void Create(HINSTANCE _hInst);
	void Run();

public:
	template <class Mgr>
	shared_ptr<Mgr> GetManager()
	{
		assert(is_base_of<Resource, Mgr>());
		for (auto mgr : m_spManagers)
			if (auto casted = dynamic_pointer_cast<Mgr>(mgr))
				return casted;
		return nullptr;
	}

	template <class Res>
	shared_ptr<Res> GetResource()
	{
		assert(is_base_of<Resource, Res>());
		for (auto res : m_spResources)
			if (auto casted = dynamic_pointer_cast<Res>(res))
				return casted;
		return nullptr;
	}
};

