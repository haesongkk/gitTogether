#pragma once
#include "Component.h"
#include <string>
#include <d2d1.h>
#include "enum.h"

namespace CBEngine
{
	///
	/// �⺻ 2D ������ �׸��� ������
	///
	/// [2020.4.12 LeHideHome]
	class PrimitiveRenderer2D : public Component
	{
	public:
		PrimitiveRenderer2D();
		virtual ~PrimitiveRenderer2D();


		/// IComponentEventFunc Override
	public:
		virtual void Update() override;

		virtual void OnPreRender() override;
		virtual void OnRender() override;
		virtual void OnPostRender() override;

		// for debug info
	private:
		void DrawDebugAxes();	// axis�� ������

	public:
		eCollisionType primitiveType;

		float m_Radius;
		D2D1_RECT_F m_AABB;
		//COBB OBB;

	};
}

