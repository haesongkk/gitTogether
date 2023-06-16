#pragma once

#include <typeinfo>
#include <string>
#include <vector>
//#include "Component.h"

using namespace std;

///
/// ���� ������Ʈ
///
/// ������Ʈ ����� ���� ���迡���� ��ƼƼ
/// ���Ӿ�ī���� 1�� ���������� �����.
///
/// [2020.4.9 LeHideHome]
namespace CBEngine
{
	class Component;

	class GameObject
	{
	public:
		GameObject(string name);
		virtual ~GameObject();

		///--------------------------------------------------
		/// ��ƼƼ ����
	private:
		string m_Name;

	public:
		string GetName() const { return m_Name; }
		void SetName(string val) { m_Name = val; }


		///--------------------------------------------------
		/// �̺�Ʈ �Լ� ����
	public:
		void Update();
		void Render();


		///--------------------------------------------------
		/// ������Ʈ ����
	private:
		// ������Ʈ��
		vector<Component*> m_Components;

		// ������Ʈ�� �߰� �Ǵ� ȹ��
	public:
		template <typename T>
		T AddComponent(T);

		template <typename T>
		T* GetComponent();

		template <typename T>
		T GetComponent2();

	};



	template <typename T>
	T CBEngine::GameObject::AddComponent(T component)
	{
		Component* _component = dynamic_cast<Component*>(component);
		_component->gameObject = this;

		m_Components.push_back(_component);

		return component;
	}

	/// <summary>
	/// �Ϲ� ����
	/// </summary>
	template <typename T>
	T* CBEngine::GameObject::GetComponent()
	{
		for (Component* co : m_Components)
		{
			const type_info& _info = typeid(*co);
			const type_info& _infoT = typeid(T);

			//string name1 = _info.name();
			//string name2 = _infoT.name();

			// *T�� ����� �� ����.
			if (typeid(*co) == typeid(T))
			{
				return dynamic_cast<T*>(co);
			}
		}

		return nullptr;
	}

	/// <summary>
	/// ������ ���� (T�� �����ͷ� �־�� ��)
	/// </summary>
	template <typename T>
	T CBEngine::GameObject::GetComponent2()
	{
		for (Component* co : m_Components)
		{
			const type_info& _info = typeid(*co);
			const type_info& _infoT = typeid(T);

			//string name1 = _info.name();
			//string name2 = _infoT.name();

			// ���̳��� ĳ��Ʈ�� ��� �߻� ���ɼ��� ����.
			// (������ �ϳ��� ��ƼƼ�� ������Ʈ�� �� ���� ������)
			T _co = dynamic_cast<T>(co);
			if (NULL != _co)
			{
				return _co;
			}
		}

		return nullptr;
	}

}



/*

1. ������ ������Ʈ�� �� �� Add���� �ʵ��� (MonoBehavior�� Mono�� ���⼭ ������?)
2. ������ C++���۷����� �� �� ������?
3. �Ϲ�ȭ(���ø�) ���� �߻�ȭ�� ���� �������� ������?
	=> ���ø��� ������� ������, �ܺο��� GetComponent()������ ���� ��, ĳ������ �ؼ� ��� �Ѵ�.

*/