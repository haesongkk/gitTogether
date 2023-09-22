#pragma once

///
/// �������� �������̽�
///
/// Software, DX12, DX9, OpenGL��
///
/// [2021.02.06 LeHideHome]

class I3DRenderer abstract
{
public:
	I3DRenderer() {}
	virtual ~I3DRenderer() {};

	// ������ �ʱ�ȭ�Ѵ�.
	virtual bool Initialize(int hinst, int hWnd, int screenWidth, int screenHeight) abstract;


	// ������Ʈ
	virtual void Update(float deltaTime) abstract;

	// ������ ����
	virtual void BeginRender() abstract;

	// ������ �� (����ۿ��� ȭ������ Blit, �������ø���, Present��...)
	virtual void EndRender() abstract;

	// ������ �����Ѵ�.
	virtual void Finalize() abstract;



public:
	// ������ ���� �� �׽�Ʈ �Լ�
	virtual void Draw_Test() abstract;

	virtual void OnMouseDown(int x, int y) abstract;
	virtual void OnMouseUp(int x, int y) abstract;
	virtual void OnMouseMove(int btnState, int x, int y) abstract;
};