#pragma once

#include <typeinfo>
#include <string>
#include <vector>
#include "CVECTOR2.h"

/// 게임 오브젝트 (전통적인 포함/상속기반)
///
/// 게임 내에 등장하는 모든 오브젝트들의 부모 클래스
/// 변환(Transform)과 그리기(Renderer)를 가지고 있다.
/// 다형성 구현의 부모 오브젝트이므로 인스턴스화 되어야 한다. (abstract은 적합치 않다)
/// 
/// 포함 구조를 설명하기 위해서 Attach함수들을 추가하고 예제를 생성함.
///
/// [2021.01.31 LeHideHome]
namespace Traditional
{
	using namespace std;

	class Transform;
	class Renderer;
	class CircleRenderer;

	class GameObject
	{
	public:
		GameObject(string name);
		virtual ~GameObject();


		///--------------------------------------------------
		/// 이벤트 관련
		virtual void Update();
		virtual void Render();

		///--------------------------------------------------
		/// 게임 오브젝트 고유
	protected:
		string m_Name;

	public:
		string GetName() const { return m_Name; }
		void SetName(string val) { m_Name = val; }

	protected:
		// 내부에 포함되어 있는 구성 요소
		// (포함-composition. 추후 다중상속이나 컴포넌트로 진화)
		Transform* m_pTransform;			// 정적으로 포함되어 있으므로 상황에 따라 떼거나 붙일 수 없다.
		CircleRenderer* m_pRenderer;

	public:
		// 1. 자체적으로 생성하는 경우
		void CreateTransform(CVECTOR2 position, float angle, CVECTOR2 scale);
		void CreateCircleRenderer(float radius);

		// 2. 외부에서 붙이는 경우 (Factory, Builder등의 Manager class가 있을 경우)
		void AttachTransform(Transform* val);
		void AttachRenderer(CircleRenderer* val);

		// getter/setter
		Transform* GetTransform() const { return m_pTransform; }
		CircleRenderer* GetRenderer() const { return m_pRenderer; }

	};

}
