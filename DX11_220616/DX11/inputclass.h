#pragma once

// DirectInput Ʃ�丮���� �ϱ� ������ �����쿡�� �����ϴ� �Է��� ����� ���̴�.
// DirectInput�� �ξ�����!!!
// InputClass�� ������ Ű���� �Է��� ó����
// �� Ŭ������ SystemClass::MessageHandler �Լ��κ��� �Է��� ���޹���
// �� ��ü�� Ű���� �迭�� �ִ� �� Ű�� ���¸� ����
// ���� Ű���� ���¿� ���� ���ǰ� ������ �� ��ü�� ���� �� Ű�� �����ִ����� ���� ������
// �˷���
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

