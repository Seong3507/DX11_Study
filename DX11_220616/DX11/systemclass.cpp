#include "systemclass.h"

SystemClass::SystemClass()
{
	// ��ü���� �����͸� null�� �ʱ�ȭ ��
	// �ʱ�ȭ�� �����ϸ� ��ٷ� Shutdown�Լ��� ȣ���ϰ� �Ǵµ� �� �Լ�������
	// ��ü�� null�� �ƴ϶�� �ùٸ��� ������ ��ü�� ����ϰ� �� ��ü�� �޸𸮸�
	// �����ؾ� �Ѵٰ� �Ǵ��ϰ� ������ ������ �����Ϸ��� �ϱ� ����
	m_Input = nullptr;
	m_Graphics = nullptr;
}

// ���� �����ڿ� �Ҹ��ڴ� �ϴ� ���д�.
// ���� �Ҹ��ڿ����� �ƹ��� ��ü ������ ���� �ʴµ� ��ſ� ��� ���� �۾���
// Shutdown�Լ����� �ϵ��� �� ���̱� �����̴�.
// �� ������ �Ҹ��� ȣ���� �ùٸ��� ���� �ʴ´ٰ� �����ϱ� �����̴�.
// ExitThread()�� ���� �Ϻ� ������ �Լ��� �ı��ڸ� ȣ������ ������ �޸� ������ �߻���Ű�� ������
// �˰� �ִ�.

SystemClass::SystemClass(const SystemClass&)
{

}

SystemClass::~SystemClass()
{

}

// Initialize �Լ����� ���ø����̼��� ��� �ʱ�ȭ �۾��� �����Ѵ�.
// �켱 InitializeWindows �Լ��� ȣ���Ͽ� ���ø����̼��� ����� �����츦 �����Ѵ�
// ���� m_Input�� m_Graphics ��ü�� �ʱ�ȭ�Ͽ� ������ �Է��� �޾Ƶ��̰�
// ȭ�鿡 �׷����� �׸� �� �ֵ��� �Ѵ�.
bool SystemClass::Initialize()
{
	int screenWidth, screenHegiht;
	bool result;

	// �Լ��� ���̿� �ʺ� �����ϱ� ���� ������ 0���� �ʱ�ȭ
	screenWidth = 0;
	screenHegiht = 0;

	// �������� api�� ����Ͽ� �ʱ�ȭ
	InitializeWindows(screenWidth, screenHegiht);

	// input ��ü�� ����, �� ��ü�� �����κ��� �巯���� Ű���� �Է��� ó���ϱ� ���� ���
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Input ��ü�� �ʱ�ȭ
	m_Input->Initialize();

	// graphics ��ü�� ����, �� ��ü�� �� ���ø����̼��� ��� �׷��� ��Ҹ� �׸��� ���� ��
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// graphic ��ü�� �ʱ�ȭ
	result = m_Graphics->Initialize(screenWidth, screenHegiht, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}


// Shutdown �Լ��� ��ü�� ����
// �̰��� ����Ǹ鼭 m_Graphics�� m_Input ��ü�� ���õ� ��� �͵��� ��ȯ�ϸ� �������
// �� ������� ���õ� �ڵ�鵵 ����
void SystemClass::Shutdown()
{
	// Graphics ��ü�� ��ȯ
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Input ��ü�� ��ȯ
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// â�� �����Ŵ
	ShutdownWindows();

	return;
}

// Run �Լ��� ���α׷��� ����� ������ ������ ���鼭 ���ø����̼��� ��� �۾��� ó��
// ���ø����̼��� ��� �۾��� �� �������� �Ҹ��� Frame �Լ����� ����
// ���ø����̼��� �ٸ� �κ��� �ۼ��� ������ �׻� ������ ��� ����ؾ� �ϴ� �߿��� ����
// �̷��� ���� ����
/* 
	while ������� ���� ����
		�������� �ý��� �޽����� Ȯ��
		�޽��� ó��
		���ø����̼��� �۾�
		������ �۾��� ���α׷��� ���Ḧ ���ϴ��� Ȯ��
*/

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// �޼��� ����ü�� �ʱ�ȭ
	ZeroMemory(&msg, sizeof(MSG));

	// �����κ��� ���� �޽����� ���� ������ ����
	done = false;
	while (!done)
	{
		// ������ �޽����� ó��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// �����쿡�� ���ø����̼��� ���Ḧ ��û�ϴ� ��� ��������
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// �� �ܿ��� Frame �Լ��� ó��
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}

	return;
}


// Frame �Լ��� ���ø����̼��� ��� �۾��� ó���Ǵ� ��
// �ܼ��� m_Input ��ü�� ���� ������ ESCŰ�� ���� �����ϱ⸦ ���ϴ°��� üũ�ϴ� ��
// ���� ������ �ʾҴٸ� m_Graphics ��ü�� ���� ȭ�鿡 �׸��� �۾��� ����
// ���ø����̼��� �� Ŀ���ٸ� �� �ڵ嵵 Ȯ��
bool SystemClass::Frame()
{
	bool result;

	// ������ ESCŰ�� ���� ���ø����̼��� �����ϱ⸦ ���ϴ��� Ȯ��
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// graphics��ü�� �۾��� ó��
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Ű���尡 Ű�� ���ȴ��� Ȯ��
	case WM_KEYDOWN:
		// Ű�� ���ȴٸ� input��ü�� �� ����� �����Ͽ� ����ϵ��� ����
		m_Input->KeyDown((unsigned int)wparam);
		return 0;


		// Ű������ ���� Ű�� ���������� Ȯ��
	case WM_KEYUP:
		// Ű�� �������ٸ� input��ü�� �� ����� �����Ͽ� �� Ű�� ������� �Ѵ�.
		m_Input->KeyUp((unsigned int)wparam);
		return 0;

	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}

}

// �츮�� �������� �ϰ� �� �����츦 ����� �ڵ尡 ��
// �� �Լ��� ȣ���� �Լ����Է� screenWidth�� screenHeight ���� �ٽ� �ǵ����ֹǷ� �� ����
// ���ø����̼ǿ��� Ȱ�� ����
// �츮�� ���⼭ ��İ� �ƹ��� ��輱�� ���� �⺻ �������� �����츦 ����
// �׸��� ���������� FULL_SCREEN�� ���� ���� �׳� ���� �����찡 �� ����, �ƴϸ�
// Ǯ ��ũ���� �����츦 ���� �� �ִ�.
// ���� �� ������ ���� false��� ���� 800x600ũ���� �����츦 ����� ȭ�� ����� ��ġ��Ŵ
// FULL_SCREEN ���� ������ graphicsclass.h�� ���ʿ� ����Ǿ� ����
// �� SystemClass��������� �ƴ϶� graphicsclass.h�� �־�� �ϴ����� ���߿� �˰Ե�
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// �ܺ� �����͸� �� ��ü�� ����
	ApplicationHandle = this;

	// �� ���ø����̼��� �ν��Ͻ��� ������
	m_hinstance = GetModuleHandle(NULL);

	// ���ø����̼��� �̸��� ����
	m_applicationName = L"Engine";

	// ������ Ŭ������ �⺻ �������� ����
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= wndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hinstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm			= wc.hIcon;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= m_applicationName;
	wc.cbSize			= sizeof(WNDCLASSEX);


	// ������ Ŭ������ ���
	RegisterClassEx(&wc);

	// ����� ȭ���� �ػ󵵸� �˾ƿ´�.
	screenWidth		= GetSystemMetrics(SM_CXSCREEN);
	screenHeight	= GetSystemMetrics(SM_CYSCREEN);

	// Ǯ��ũ�� ��� ������ ���� ���� ȭ�� ������ �Ѵ�.
	if (FULL_SCREEN)
	{
		// ���� Ǯ��ũ�� ����� ȭ�� ũ�⸦ ����ũ�� ũ�⿡ ���߰� ������ 32bit�� �Ѵ�.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Ǯ��ũ���� �´� ���÷��̸� ����
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);


		// �������� ��ġ�� ȭ���� ���� ���� ����
		posX = posY = 0;
	}
	else
	{
		// ������ ����� 800x600 ũ�⸦ ������ ��
		screenWidth = 800;
		screenHeight = 600;

		// â�� ������� �߾ӿ� ������ ��
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// ������ ���� ������ â�� ����� �� �ڵ��� �����´�.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ���� ��
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// ���콺 Ŀ���� ǥ������ ����
	ShowCursor(false);

	return;
}

// ShutdownWindowns�� ȭ�� ������ �ǵ����� ������� �� �ڵ���� ��ȯ
void SystemClass::ShutdownWindows()
{
	// ���콺 Ŀ���� ǥ��
	ShowCursor(true);

	// Ǯ��ũ�� ��带 �������� �� ���÷��� ������ �ٲ�
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(nullptr, 0);
	}

	// â�� ����
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ���ø����̼� �ν��Ͻ��� ����
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// �� Ŭ������ ���� �ܺ� ������ ������ ����
	ApplicationHandle = nullptr;

	return;
}

// ������ �ý��ۿ��� �޼����� ������ ��, �Ƹ� ���� InitializeWindows �Լ�����
// ������ Ŭ������ �ʱ�ȭ �� �� wc.lpfnWndProc = WndProc�� ���� WndProc�� ���縦 ��ġç�Ŵ�
// MessageHandler���� ó���� �޼����� �޾ƾ� �ϱ� ������ WndProc �Լ��� �� Ŭ���� ���� �ȿ� ����
// SystemClass���� �������� �޼����� ����ä�� ���� �����ϰ� �� �ְ� �ڵ� ���� �����ϰ� ��

LRESULT CALLBACK wndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// �����찡 ���ŵǾ����� Ȯ��
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// �����찡 �������� Ȯ��
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

		// �ٸ� ��� �޼������� system Ŭ������ �޼��� ó���⿡ ����
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}
