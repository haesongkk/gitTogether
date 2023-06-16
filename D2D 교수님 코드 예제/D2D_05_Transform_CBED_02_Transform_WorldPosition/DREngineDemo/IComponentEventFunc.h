#pragma once

namespace CBEngine
{
	///
	/// Object�� �̺�Ʈ �Լ� �������̽�
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

