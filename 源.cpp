#include <iostream>
#include <Windows.h>
#include <cstdlib>
#define Mouse(VK_RBUTTON) ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? 1:0)
#define Keyboard(VK_RETURN) ((GetAsyncKeyState(VK_RETURN) & 0x8000) ? 1:0)

using namespace std;
POINT p;
HWND h = GetForegroundWindow();//�����ϳ����Ƴ����ٱ༭ģʽ

int dingCount = 0;

static BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam) {
	int length = GetWindowTextLengthA(hWnd);
	char* buffer = new char[length + 1];
	GetWindowTextA(hWnd, buffer, length + 1);
	std::string windowTitle(buffer);

	// List visible windows with a non-empty title
	if (IsWindowVisible(hWnd) && windowTitle == "����") {
		dingCount++;
	}
	return TRUE;
}

void TopWindow(HWND& hWnd) {										      //����̨�ö�
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

int main() {
	int x = 0;
	int y = 0;
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);//�����ϳ����Ƴ����ٱ༭ģʽ
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hStdin, mode);

	cout << "�뽫����ȫ��������һ�ֱ���ƿ��λ��,��enterȷ��" << endl;

	HWND hWnd = GetForegroundWindow();
	while (1) {
		TopWindow(hWnd);//����̨�ö�
		if (Mouse(VK_RBUTTON)) {  			//��ȡ����ڴ����һ���λ��
			POINT p;
			GetCursorPos(&p);
			printf("Mouse (%d,%d)\n", p.x, p.y);
			x = p.x;
			y = p.y;
		}
	if (Keyboard(VK_RETURN)) {  			//����enter����ѭ��
		cout << "��ǰ����:" << x << " " << y << endl;
		break;
	}
	Sleep(100);
	}
	cout << "����ʼ����...\n" << "����esc���˳�" << endl;

	INPUT input = { 0 };		//���ϳ��ĳ�ʼ��
	input.type = INPUT_MOUSE;
	input.mi.dx = static_cast<long>(65535.0f / (GetSystemMetrics(SM_CXSCREEN) - 1) * x);
	input.mi.dy = static_cast<long>(65535.0f / (GetSystemMetrics(SM_CYSCREEN) - 1) * y);
	input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	while (true) {
		Sleep(5000);
		::SendInput(1, &input, sizeof(INPUT));//������

		if (Keyboard(VK_ESCAPE)) {//����esc����ѭ��
			cout << "�ѽ�������\a" << endl;
			break;
		}
		dingCount = 0;
		EnumWindows(enumWindowCallback, NULL);
		if (dingCount > 1) {
			cout << "ֱ���ѿ�ʼ" << endl;
		} else {
			continue;
		}
		while (true) {
			dingCount = 0;
			EnumWindows(enumWindowCallback, NULL);
			if (dingCount < 2) {
				cout << "ֱ���ѽ���" << endl;
				break;
			}
		}
	}




	system("pause");
	return 0;
}