#pragma once

// DirectInput 튜토리얼을 하기 전까지 윈도우에서 제공하는 입력을 사용할 것이다.
// DirectInput이 훨씬빠름!!!
// InputClass는 유저의 키보드 입력을 처리함
// 이 클래스는 SystemClass::MessageHandler 함수로부터 입력을 전달받음
// 이 객체는 키보드 배열에 있는 각 키의 상태를 저장
// 만약 키보드 상태에 대한 질의가 들어오면 이 객체는 현재 이 키가 눌려있는지에 대한 정보를
// 알려줌
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();
	
	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
};

