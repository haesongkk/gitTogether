#pragma once

#include <string>
#include "Object.h"
#include "GameObject.h"	// 템플릿 상호참조 주의. 

namespace CBEngine
{
	///
	/// 컴포넌트
	///
	/// 컴포넌트는 엔티티(GameObject)에 붙거나 떨어지거나 하는 것을 구현한 계층이다.
	///
	/// [2020.4.12 LeHideHome]
	class Component : public Object
	{
	public:
		Component();
		virtual ~Component();


	public:
		std::string name;



		/// 컴포넌트 관련

		// 컴포넌트를 추가 또는 획득
	public:
		template <typename T>
		T AddComponent(T);

		template <typename T>
		T* GetComponent();

		template <typename T>
		T GetComponent2();

	public:
		// 컴포넌트가 붙어있는 게임오브젝트.
		// 컴포넌트는 게임오브젝트(엔티티)에 항상 붙어 있다.
		GameObject* gameObject;

	};

	template <typename T>
	T CBEngine::Component::GetComponent2()
	{
		if (gameObject != nullptr)
		{
			return gameObject->template GetComponent2<T>();

			// 이제 템플릿 한정 키워드를 쓰지 않아도 된다. (컴파일러인지 C++ 14인지는 정확하지 않음)
			//return gameObject->GetComponent2<T>();
		}

		return nullptr;
	}

	template <typename T>
	T* CBEngine::Component::GetComponent()
	{
		if (gameObject != nullptr)
		{
			return gameObject->template GetComponent<T>();
		}

		return nullptr;
	}

	template <typename T>
	T CBEngine::Component::AddComponent(T)
	{
		if (gameObject != nullptr)
		{
			return gameObject->template AddComponent<T>();
		}

		return nullptr;
	}

}

