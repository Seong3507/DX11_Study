#pragma once

// ���� ������ �ʴ� API���� ��� �ִ� Win32����� �������� �������� ���� �ӵ��� ���̱� ����
#define WIN32_LEAN_AND_MEAN

// �츮�� �����츦 ����/���� �ؾ��ϴ� �Լ����� ȣ���� �� ������
// �ٸ� �پ��� win32�Լ����� ����� �� ����
#include <windows.h>

// systemclass���� ����ϱ� ���� �����ӿ�ũ�� �ٸ� �� Ŭ������ ����� ����
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

// wndProc�Լ��� ApplicationHandle ������ ���� Ŭ���� ���� �ȿ� �����Ͽ�
// �������� �ý��� �޽������� MessageHandler�� ������ �� �ְ� ��
static LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

