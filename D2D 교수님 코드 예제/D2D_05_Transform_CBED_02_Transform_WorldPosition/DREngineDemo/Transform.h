#pragma once
#include "Component.h"
#include <d2d1.h>
#include "CVECTOR2.h"
#include "DRD2DEngine.h"

namespace CBEngine
{
	class Transform : public Component
	{
	public:
		Transform();
		virtual ~Transform();


		/// IComponentEventFunc Override
	public:
		virtual void Update() override;

		virtual void OnPreRender() override;
		virtual void OnRender() override;
		virtual void OnPostRender() override;


		// 1. 3종의 변환 행렬을 만드는 함수
		// 2. 위치, 회전값, 스케일값을 지정 (그리고 바로 적용되어 변환행렬까지 적용되어야 함)
		// 3. 위치이동(Translate), 회전(Rotate), 크기변환(Scale) 조작함수

		///--------------------------------------------------
		/// 위치, 회전 값, 스케일 값 (로컬)
	private:
		CVECTOR2 LocalPosition;
		float LocalRotation;
		CVECTOR2 LocalScale;

	public:
		// 로컬 좌표계 - 위치, 회전값, 스케일값 바로 적용
		void SetLocalPosition(CVECTOR2 position);
		void SetLocalRotation(float rotation);
		void SetLocalScale(CVECTOR2 scale);
		CVECTOR2 GetLocalPosition() const;
		float GetLocalRotation() const;
		CVECTOR2 GetLocalScale() const;


		// 월드 좌표계
		void SetPosition(CVECTOR2 position);
		void SetRotation(float rotation);
		void SetScale(CVECTOR2 scale);
		CVECTOR2 GetPosition();
		float GetRotation();
		CVECTOR2 GetScale();

		///--------------------------------------------------
		/// 이동하기, 회전하기
	public:
		void Translate(CVECTOR2 translation);
		void Rotate(float angle);


		///--------------------------------------------------
		/// 변환 행렬 관련
	private:
		// 변환 행렬 생성
		void MakeTranslateMatrix(CVECTOR2 delta);				// 이동
		void MakeRotationMatrix(float angle, CVECTOR2 axis);	// 회전
		void MakeScaleMatrix(CVECTOR2 size, CVECTOR2 center);	// 크기

		// 변환 행렬을 초기화한다.
		void Identify();

	public:	// test
		// 최종 변환 행렬을 만들어준다.
		void MakeTransformMatrix();
		D2D1::Matrix3x2F GetTransformMatrix() const { return m_TransformMatrix; }

	private:
		// Transform Matrix
		///D2D1_MATRIX_3X2_F m_TransformMatrix;
		D2D1::Matrix3x2F m_TransformMatrix;

		// 3개 성분으로 나누어진 형태의 TM
		D2D1::Matrix3x2F m_TranslateMatrix;
		D2D1::Matrix3x2F m_RotationMatrix;
		D2D1::Matrix3x2F m_ScaleMatrix;


		// 변환 행렬을 엔진에 적용
	public:
		void SetTransform();

		// 변환 행렬에서 이동, 회전, 크기값 추출
		CVECTOR2 GetPositionFromTM(D2D1::Matrix3x2F TM);
		float GetRotationFromTM(D2D1::Matrix3x2F TM);
		CVECTOR2 GetScaleFromTM(D2D1::Matrix3x2F TM);



		///--------------------------------------------------
		/// 부모-자식 관련
	private:
		Transform* m_pParent;
		D2D1::Matrix3x2F m_ParentsTM;

	public:
		//CBEngine::GameObject* SetParent() const { return m_pParent; }
		void SetParent(CBEngine::Transform* parent);
		string GetParentName();

	};
}

/*

부모-자식 관계를 만들어야 함

*/