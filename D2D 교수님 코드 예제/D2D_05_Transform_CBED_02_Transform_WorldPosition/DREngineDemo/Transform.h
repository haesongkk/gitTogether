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


		// 1. 3���� ��ȯ ����� ����� �Լ�
		// 2. ��ġ, ȸ����, �����ϰ��� ���� (�׸��� �ٷ� ����Ǿ� ��ȯ��ı��� ����Ǿ�� ��)
		// 3. ��ġ�̵�(Translate), ȸ��(Rotate), ũ�⺯ȯ(Scale) �����Լ�

		///--------------------------------------------------
		/// ��ġ, ȸ�� ��, ������ �� (����)
	private:
		CVECTOR2 LocalPosition;
		float LocalRotation;
		CVECTOR2 LocalScale;

	public:
		// ���� ��ǥ�� - ��ġ, ȸ����, �����ϰ� �ٷ� ����
		void SetLocalPosition(CVECTOR2 position);
		void SetLocalRotation(float rotation);
		void SetLocalScale(CVECTOR2 scale);
		CVECTOR2 GetLocalPosition() const;
		float GetLocalRotation() const;
		CVECTOR2 GetLocalScale() const;


		// ���� ��ǥ��
		void SetPosition(CVECTOR2 position);
		void SetRotation(float rotation);
		void SetScale(CVECTOR2 scale);
		CVECTOR2 GetPosition();
		float GetRotation();
		CVECTOR2 GetScale();

		///--------------------------------------------------
		/// �̵��ϱ�, ȸ���ϱ�
	public:
		void Translate(CVECTOR2 translation);
		void Rotate(float angle);


		///--------------------------------------------------
		/// ��ȯ ��� ����
	private:
		// ��ȯ ��� ����
		void MakeTranslateMatrix(CVECTOR2 delta);				// �̵�
		void MakeRotationMatrix(float angle, CVECTOR2 axis);	// ȸ��
		void MakeScaleMatrix(CVECTOR2 size, CVECTOR2 center);	// ũ��

		// ��ȯ ����� �ʱ�ȭ�Ѵ�.
		void Identify();

	public:	// test
		// ���� ��ȯ ����� ������ش�.
		void MakeTransformMatrix();
		D2D1::Matrix3x2F GetTransformMatrix() const { return m_TransformMatrix; }

	private:
		// Transform Matrix
		///D2D1_MATRIX_3X2_F m_TransformMatrix;
		D2D1::Matrix3x2F m_TransformMatrix;

		// 3�� �������� �������� ������ TM
		D2D1::Matrix3x2F m_TranslateMatrix;
		D2D1::Matrix3x2F m_RotationMatrix;
		D2D1::Matrix3x2F m_ScaleMatrix;


		// ��ȯ ����� ������ ����
	public:
		void SetTransform();

		// ��ȯ ��Ŀ��� �̵�, ȸ��, ũ�Ⱚ ����
		CVECTOR2 GetPositionFromTM(D2D1::Matrix3x2F TM);
		float GetRotationFromTM(D2D1::Matrix3x2F TM);
		CVECTOR2 GetScaleFromTM(D2D1::Matrix3x2F TM);



		///--------------------------------------------------
		/// �θ�-�ڽ� ����
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

�θ�-�ڽ� ���踦 ������ ��

*/