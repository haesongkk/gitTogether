#pragma once

#include <string>
#include "Object.h"
#include "GameObject.h"	// ���ø� ��ȣ���� ����. 

namespace CBEngine
{
	///
	/// ������Ʈ
	///
	/// ������Ʈ�� ��ƼƼ(GameObject)�� �ٰų� �������ų� �ϴ� ���� ������ �����̴�.
	///
	/// [2020.4.12 LeHideHome]
	class Component : public Object
	{
	public:
		Component();
		virtual ~Component();


	public:
		std::string name;



		/// ������Ʈ ����

		// ������Ʈ�� �߰� �Ǵ� ȹ��
	public:
		template <typename T>
		T AddComponent(T);

		template <typename T>
		T* GetComponent();

		template <typename T>
		T GetComponent2();

	public:
		// ������Ʈ�� �پ��ִ� ���ӿ�����Ʈ.
		// ������Ʈ�� ���ӿ�����Ʈ(��ƼƼ)�� �׻� �پ� �ִ�.
		GameObject* gameObject;

	};

	template <typename T>
	T CBEngine::Component::GetComponent2()
	{
		if (gameObject != nullptr)
		{
			return gameObject->template GetComponent2<T>();

			// ���� ���ø� ���� Ű���带 ���� �ʾƵ� �ȴ�. (�����Ϸ����� C++ 14������ ��Ȯ���� ����)
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

