#pragma once

#include <typeinfo>
#include <string>
#include <vector>
#include "CVECTOR2.h"

/// ���� ������Ʈ (�������� ����/��ӱ��)
///
/// ���� ���� �����ϴ� ��� ������Ʈ���� �θ� Ŭ����
/// ��ȯ(Transform)�� �׸���(Renderer)�� ������ �ִ�.
/// ������ ������ �θ� ������Ʈ�̹Ƿ� �ν��Ͻ�ȭ �Ǿ�� �Ѵ�. (abstract�� ����ġ �ʴ�)
/// 
/// ���� ������ �����ϱ� ���ؼ� Attach�Լ����� �߰��ϰ� ������ ������.
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
		/// �̺�Ʈ ����
		virtual void Update();
		virtual void Render();

		///--------------------------------------------------
		/// ���� ������Ʈ ����
	protected:
		string m_Name;

	public:
		string GetName() const { return m_Name; }
		void SetName(string val) { m_Name = val; }

	protected:
		// ���ο� ���ԵǾ� �ִ� ���� ���
		// (����-composition. ���� ���߻���̳� ������Ʈ�� ��ȭ)
		Transform* m_pTransform;			// �������� ���ԵǾ� �����Ƿ� ��Ȳ�� ���� ���ų� ���� �� ����.
		CircleRenderer* m_pRenderer;

	public:
		// 1. ��ü������ �����ϴ� ���
		void CreateTransform(CVECTOR2 position, float angle, CVECTOR2 scale);
		void CreateCircleRenderer(float radius);

		// 2. �ܺο��� ���̴� ��� (Factory, Builder���� Manager class�� ���� ���)
		void AttachTransform(Transform* val);
		void AttachRenderer(CircleRenderer* val);

		// getter/setter
		Transform* GetTransform() const { return m_pTransform; }
		CircleRenderer* GetRenderer() const { return m_pRenderer; }

	};

}
