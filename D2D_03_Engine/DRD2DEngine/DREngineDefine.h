#pragma once

/// 스프라이트의 추상적인 정보
/// 내부 구현은 다를 수 있다.
class DRSprite
{
public:
	DRSprite(int index, int width, int height, int delayFrame);
	virtual ~DRSprite();

	// 원본과 연결?
	// 어찌됐든 가져오는 방법

public:
	// 연결할 특정 인덱스(키)
	int Index;

	// 비트맵의 폭, 높이
	int Width;
	int Height;

	// 프레임 제어 관련
	int DelayFrame;
};


