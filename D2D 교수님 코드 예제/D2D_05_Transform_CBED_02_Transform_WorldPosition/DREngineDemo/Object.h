#pragma once

#include "IComponentEventFunc.h"

namespace CBEngine
{
	///
	/// 게임오브젝트와 컴포넌트의 기반이 되는 최상위 클래스
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


		// 오브젝트의 생성, 삭제, 복사, ID 등
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