#pragma once

/// ��������Ʈ�� �߻����� ����
/// ���� ������ �ٸ� �� �ִ�.
class DRSprite
{
public:
	DRSprite(int index, int width, int height, int delayFrame);
	virtual ~DRSprite();

	// ������ ����?
	// ����Ƶ� �������� ���

public:
	// ������ Ư�� �ε���(Ű)
	int Index;

	// ��Ʈ���� ��, ����
	int Width;
	int Height;

	// ������ ���� ����
	int DelayFrame;
};


