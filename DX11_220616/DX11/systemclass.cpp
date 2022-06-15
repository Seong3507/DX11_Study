#include "systemclass.h"

SystemClass::SystemClass()
{
	// 객체들의 포인터를 null로 초기화 함
	// 초기화에 실패하면 곧바로 Shutdown함수를 호출하게 되는데 이 함수에서는
	// 객체가 null이 아니라면 올바르게 생성된 객체로 취급하고 이 객체의 메모리를
	// 정리해야 한다고 판단하고 실제로 정리를 수행하려고 하기 때문
	m_Input = nullptr;
	m_Graphics = nullptr;
}

// 복사 생성자와 소멸자는 일단 비어둔다.
// 또한 소멸자에서는 아무런 객체 정리를 하지 않는데 대신에 모든 정리 작업을
// Shutdown함수에서 하도록 할 것이기 때문이다.
// 그 이유는 소멸자 호출이 올바르게 되지 않는다고 생각하기 때문이다.
// ExitThread()와 같은 일부 윈도우 함수는 파괴자를 호출하지 않으며 메모리 누수를 발생시키는 것으로
// 알고 있다.

SystemClass::SystemClass(const SystemClass&)
{

}

SystemClass::~SystemClass()
{

}

// Initialize 함수에서 어플리케이션의 모든 초기화 작업을 수행한다.
// 우선 InitializeWindows 함수를 호출하여 어플리케이션이 사용할 윈도우를 생성한다
// 또한 m_Input과 m_Graphics 객체를 초기화하여 유저의 입력을 받아들이고
// 화면에 그래픽을 그릴 수 있도록 한다.
bool SystemClass::Initialize()
{
	int screenWidth, screenHegiht;
	bool result;

	// 함수에 높이와 너비를 전달하기 전에 변수를 0으로 초기화
	screenWidth = 0;
	screenHegiht = 0;

	// 윈도우즈 api를 사용하여 초기화
	InitializeWindows(screenWidth, screenHegiht);

	// input 객체를 생성, 이 객체는 유저로부터 드러오는 키보드 입력을 처리하기 이해 사용
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Input 객체를 초기화
	m_Input->Initialize();

	// graphics 객체를 생성, 이 객체는 이 어플리케이션의 모든 그래픽 요소를 그리는 일을 함
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// graphic 객체를 초기화
	result = m_Graphics->Initialize(screenWidth, screenHegiht, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}


// Shutdown 함수는 객체를 정리
// 이것은 종료되면서 m_Graphics와 m_Input 객체와 관련된 모든 것들을 반환하며 윈도우와
// 그 윈도우와 관련된 핸들들도 정리
void SystemClass::Shutdown()
{
	// Graphics 객체를 반환
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Input 객체를 반환
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// 창을 종료시킴
	ShutdownWindows();

	return;
}

// Run 함수는 프로그램이 종료될 때까지 루프를 돌면서 어플리케이션의 모든 작업을 처리
// 어플리케이션의 모든 작업은 매 루프마다 불리는 Frame 함수에서 수행
// 어플리케이션의 다른 부분을 작성할 때에도 항상 마음에 담고 고려해야 하는 중요한 개념
// 이렇게 만들 예정
/* 
	while 종료되지 않은 동안
		윈도우의 시스템 메시지를 확인
		메시지 처리
		어플리케이션의 작업
		유저가 작업중 프로그램의 종료를 원하는지 확인
*/

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// 메세지 구조체를 초기화
	ZeroMemory(&msg, sizeof(MSG));

	// 유저로부터 종료 메시지를 받을 때까지 루프
	done = false;
	while (!done)
	{
		// 윈도우 메시지를 처리
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 윈도우에서 어플리케이션의 종료를 요청하는 경우 빠져나감
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// 그 외에는 Frame 함수를 처리
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}

	return;
}


// Frame 함수는 어플리케이션의 모든 작업이 처리되는 곳
// 단순히 m_Input 객체를 통해 유저가 ESC키를 눌러 종료하기를 원하는가만 체크하는 중
// 만약 누르지 않았다면 m_Graphics 객체를 통해 화면에 그리는 작업을 수행
// 어플리케이션이 더 커진다면 이 코드도 확장
bool SystemClass::Frame()
{
	bool result;

	// 유저가 ESC키를 눌러 어플리케이션을 종료하기를 원하는지 확인
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// graphics객체의 작업을 처리
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
		// 키보드가 키가 눌렸는지 확인
	case WM_KEYDOWN:
		// 키가 눌렸다면 input객체에 이 사실을 전달하여 기록하도록 관리
		m_Input->KeyDown((unsigned int)wparam);
		return 0;


		// 키보드의 눌린 키가 떼어졌는지 확인
	case WM_KEYUP:
		// 키가 떼어졌다면 input객체에 이 사실을 전달하여 이 키를 해제토록 한다.
		m_Input->KeyUp((unsigned int)wparam);
		return 0;

	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}

}

// 우리가 렌더링을 하게 될 윈도우를 만드는 코드가 들어감
// 이 함수는 호출한 함수에게로 screenWidth와 screenHeight 값을 다시 되돌려주므로 이 값을
// 어플리케이션에서 활용 가능
// 우리는 여기서 까맣고 아무런 경계선이 없는 기본 설정으로 윈도우를 만듬
// 그리고 전역변수인 FULL_SCREEN의 값에 따라 그냥 작은 윈도우가 될 수도, 아니면
// 풀 스크린의 윈도우를 만들 수 있다.
// 만약 이 변수의 값이 false라면 단지 800x600크기의 윈도우를 만들어 화면 가운데에 위치시킴
// FULL_SCREEN 전역 변수는 graphicsclass.h의 윗쪽에 선언되어 있음
// 왜 SystemClass헤더파일이 아니라 graphicsclass.h에 있어야 하는지는 나중에 알게됨
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// 외부 포인터를 이 객체로 설정
	ApplicationHandle = this;

	// 이 어플리케이션의 인스턴스를 가져옴
	m_hinstance = GetModuleHandle(NULL);

	// 어플리케이션의 이름을 설정
	m_applicationName = L"Engine";

	// 윈도우 클래스를 기본 설정으로 맞춤
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


	// 윈도우 클래스를 등록
	RegisterClassEx(&wc);

	// 모니터 화면의 해상도를 알아온다.
	screenWidth		= GetSystemMetrics(SM_CXSCREEN);
	screenHeight	= GetSystemMetrics(SM_CYSCREEN);

	// 풀스크린 모드 변수의 값에 따라 화면 설정을 한다.
	if (FULL_SCREEN)
	{
		// 만약 풀스크린 모드라면 화면 크기를 데스크톱 크기에 맞추고 색상을 32bit로 한다.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// 풀스크린에 맞는 디스플레이를 설정
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);


		// 윈도우의 위치를 화면의 왼쪽 위로 맞춤
		posX = posY = 0;
	}
	else
	{
		// 윈도우 모드라면 800x600 크기를 가지게 함
		screenWidth = 800;
		screenHeight = 600;

		// 창을 모니터의 중앙에 오도록 함
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// 설정한 것을 가지고 창을 만들고 그 핸들을 가져온다.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// 윈도우를 화면에 표시하고 포커스를 줌
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// 마우스 커서를 표시하지 않음
	ShowCursor(false);

	return;
}

// ShutdownWindowns는 화면 설정을 되돌리고 윈도우와 그 핸들들을 반환
void SystemClass::ShutdownWindows()
{
	// 마우스 커서를 표시
	ShowCursor(true);

	// 풀스크린 모드를 빠져나올 때 디스플레이 설정을 바꿈
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(nullptr, 0);
	}

	// 창을 제거
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// 어플리케이션 인스턴스를 제거
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// 이 클래스에 대한 외부 포인터 참조를 제거
	ApplicationHandle = nullptr;

	return;
}

// 윈도우 시스템에서 메세지를 보내는 곳, 아마 위의 InitializeWindows 함수에서
// 윈도우 클래스를 초기화 할 때 wc.lpfnWndProc = WndProc를 보고 WndProc의 존재를 눈치챌거다
// MessageHandler에서 처리할 메세지를 받아야 하기 떄문에 WndProc 함수도 이 클래스 파일 안에 선언
// SystemClass에서 윈도우의 메세지를 가로채는 것을 간으하게 해 주고 코드 역시 간단하게 함

LRESULT CALLBACK wndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// 윈도우가 제거되었는지 확인
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// 윈도우가 닫히는지 확인
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

		// 다른 모든 메세지들은 system 클래스의 메세지 처리기에 전달
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}
