#pragma once
#include <string>
#include <d2d1.h>
#include "Renderer.h"
#include <string>

namespace Traditional
{
	using namespace std;

	/// 원을 그리는 랜더러
	///
	/// [2020.4.12 LeHideHome]
	class CircleRenderer : public Renderer
	{
	public:
		CircleRenderer();
		virtual ~CircleRenderer();


		/// IComponentEventFunc Override
	public:
		virtual void Update() override;

		virtual void OnPreRender() override;
		virtual void OnRender() override;
		virtual void OnPostRender() override;

		// for debug info
	private:
		void DrawDebugAxes();	// axis의 복수형

	public:
		float m_Radius;
		D2D1_RECT_F m_AABB;

	protected:
		string m_Name;

	};
}

