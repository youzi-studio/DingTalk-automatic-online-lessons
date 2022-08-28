#include <iostream>
#include <Windows.h>
#include <cstdlib>
#define Mouse(VK_RBUTTON) ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? 1:0)
#define Keyboard(VK_RETURN) ((GetAsyncKeyState(VK_RETURN) & 0x8000) ? 1:0)

using namespace std;
POINT p;
HWND h = GetForegroundWindow();//从网上抄的移除快速编辑模式

int dingCount = 0;

static BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam) {
	int length = GetWindowTextLengthA(hWnd);
	char* buffer = new char[length + 1];
	GetWindowTextA(hWnd, buffer, length + 1);
	std::string windowTitle(buffer);

	// List visible windows with a non-empty title
	if (IsWindowVisible(hWnd) && windowTitle == "钉钉") {
		dingCount++;
	}
	return TRUE;
}

void TopWindow(HWND& hWnd) {										      //控制台置顶
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

int main() {
	int x = 0;
	int y = 0;
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);//从网上抄的移除快速编辑模式
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hStdin, mode);

	cout << "请将钉钉全屏并鼠标右击直播黄框的位置,按enter确定" << endl;

	HWND hWnd = GetForegroundWindow();
	while (1) {
		TopWindow(hWnd);//控制台置顶
		if (Mouse(VK_RBUTTON)) {  			//获取鼠标在窗口右击的位置
			POINT p;
			GetCursorPos(&p);
			printf("Mouse (%d,%d)\n", p.x, p.y);
			x = p.x;
			y = p.y;
		}
	if (Keyboard(VK_RETURN)) {  			//按下enter结束循环
		cout << "当前坐标:" << x << " " << y << endl;
		break;
	}
	Sleep(100);
	}
	cout << "程序开始运行...\n" << "长按esc键退出" << endl;

	INPUT input = { 0 };		//网上抄的初始化
	input.type = INPUT_MOUSE;
	input.mi.dx = static_cast<long>(65535.0f / (GetSystemMetrics(SM_CXSCREEN) - 1) * x);
	input.mi.dy = static_cast<long>(65535.0f / (GetSystemMetrics(SM_CYSCREEN) - 1) * y);
	input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	while (true) {
		Sleep(5000);
		::SendInput(1, &input, sizeof(INPUT));//鼠标左击

		if (Keyboard(VK_ESCAPE)) {//长按esc结束循环
			cout << "已结束操作\a" << endl;
			break;
		}
		dingCount = 0;
		EnumWindows(enumWindowCallback, NULL);
		if (dingCount > 1) {
			cout << "直播已开始" << endl;
		} else {
			continue;
		}
		while (true) {
			dingCount = 0;
			EnumWindows(enumWindowCallback, NULL);
			if (dingCount < 2) {
				cout << "直播已结束" << endl;
				break;
			}
		}
	}




	system("pause");
	return 0;
}