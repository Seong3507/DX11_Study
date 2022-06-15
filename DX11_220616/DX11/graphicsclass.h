#pragma once

#include <windows.h>

// graphicsclass는 systemclass가 생성하는 또다른 객체
// 이 어플리케이션의 모든 그래픽 기능이 이 객체 안에 캡슐화 되어 있음
// 이 헤더에 풀 스크린 모드와 같은 그래픽과 관련된 전역 설정을 할 수 있음
// 현재 이 클래스는 거의 비어있지만 나중에는 모든 그래픽 객체들을 담을 것이다.

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:

};

