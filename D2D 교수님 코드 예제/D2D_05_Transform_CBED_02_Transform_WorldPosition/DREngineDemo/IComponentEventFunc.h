#pragma once

namespace CBEngine
{
	///
	/// Object의 이벤트 함수 인터페이스
	///
	/// [2020.4.12 LeHideHome]
	class IComponentEventFunc
	{
		virtual void Update() abstract;

		virtual void OnPreRender() abstract;
		virtual void OnRender() abstract;
		virtual void OnPostRender() abstract;
	};
}

