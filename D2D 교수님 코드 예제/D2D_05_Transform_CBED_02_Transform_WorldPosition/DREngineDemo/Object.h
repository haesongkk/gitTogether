#pragma once

#include "IComponentEventFunc.h"

namespace CBEngine
{
	///
	/// ���ӿ�����Ʈ�� ������Ʈ�� ����� �Ǵ� �ֻ��� Ŭ����
	///
	/// [2020.4.12 LeHideHome]
	///
	class Object
		: public IComponentEventFunc
	{
		/// Object
	public:
		Object();
		virtual ~Object();


		// ������Ʈ�� ����, ����, ����, ID ��
		//public static Object Instantiate(Object original, Transform parent, bool instantiateInWorldSpace);
		//public override bool Equals(object other);
		//public override int GetHashCode();
		//public int GetInstanceID();
		//public override string ToString();


		/// IComponentEventFunc Override
	public:
		virtual void Update() override;

		virtual void OnPreRender() override;
		virtual void OnRender() override;
		virtual void OnPostRender() override;
	};

}