#pragma once

#include <typeinfo>
#include <string>
#include <vector>
//#include "Component.h"

using namespace std;

///
/// 게임 오브젝트
///
/// 컴포넌트 기반의 엔진 설계에서의 엔티티
/// 게임아카데미 1기 교육용으로 만들다.
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
		/// 엔티티 고유
	private:
		string m_Name;

	public:
		string GetName() const { return m_Name; }
		void SetName(string val) { m_Name = val; }


		///--------------------------------------------------
		/// 이벤트 함수 관련
	public:
		void Update();
		void Render();


		///--------------------------------------------------
		/// 컴포넌트 관련
	private:
		// 컴포넌트들
		vector<Component*> m_Components;

		// 컴포넌트를 추가 또는 획득
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
	/// 일반 버전
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

			// *T를 사용할 수 없음.
			if (typeid(*co) == typeid(T))
			{
				return dynamic_cast<T*>(co);
			}
		}

		return nullptr;
	}

	/// <summary>
	/// 포인터 버전 (T를 포인터로 넣어야 함)
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

			// 다이내믹 캐스트는 비용 발생 가능성이 있음.
			// (하지만 하나의 엔티티에 컴포넌트가 몇 개나 있을까)
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

1. 동일한 컴포넌트가 두 번 Add되지 않도록 (MonoBehavior의 Mono가 여기서 왔을까?)
2. 구현을 C++레퍼런스로 할 수 없을까?
3. 일반화(템플릿) 말고 추상화로 구현 가능하지 않을까?
	=> 템플릿을 사용하지 않으면, 외부에서 GetComponent()등으로 받을 때, 캐스팅을 해서 써야 한다.

*/