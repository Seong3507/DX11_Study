#pragma once

// 자주 사용되지 않는 API들을 담고 있는 Win32헤더를 포함하지 않음으로 빌드 속도를 높이기 위함
#define WIN32_LEAN_AND_MEAN

// 우리가 윈도우를 생성/제거 해야하는 함수들을 호출할 수 있으며
// 다른 다양한 win32함수들을 사용할 수 있음
#include <windows.h>

// systemclass에서 사용하기 위한 프레임워크의 다른 두 클래스의 헤더를 포함
#include "inputclass.h"
#include "graphicsclass.h"


class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
};

// wndProc함수와 ApplicationHandle 포인터 역시 클래스 파일 안에 포함하여
// 윈도우의 시스템 메시지들을 MessageHandler로 전달할 수 있게 함
static LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

