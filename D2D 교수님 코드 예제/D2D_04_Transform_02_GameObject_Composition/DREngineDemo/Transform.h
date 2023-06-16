#pragma once
#include <d2d1.h>
#include "CVECTOR2.h"
#include "DRD2DEngine.h"
#include "IComponentEventFunc.h"
#include <string>

namespace Traditional
{
	using namespace std;

	class GameObject;

	/// ��ȯ
	/// 2���� ��ǥ���� ��ȯ�� �����
	/// 
	/// 2021.01.31 LeHide
	class Transform : public IComponentEventFunc
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
		void SetParent(Transform* parent);
		string GetParentName();

		///--------------------------------------------------
		/// �� ���� ��Ұ� ���ԵǾ� �ִ� ���ӿ�����Ʈ
	public:
		GameObject* m_pGameObject;

		string name;

	};
}
