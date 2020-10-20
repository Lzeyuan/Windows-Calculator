#include <windows.h>
#include <string>
#include <strsafe.h>
#include "basewindow.h"
#include "calculate.h"
#include <windowsx.h>
#include "resource.h"
#include "Naturalnumber.h"

#define W_WIDTH 390
#define W_HEIGHT 480

class Calculator : public MainWindow {
public:
	Calculator() :op1("0"), op2(""), opt(""), res(""), countBrackets(0) {}
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL Create(PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = W_WIDTH,
		int nHeight = W_HEIGHT,
		HWND hWndParent = 0,
		HMENU hMenu = 0);
	//TCHAR* OutText(const std::string&);
	//void ChangeWindowText(HWND hwnd, const std::string& temp);
private:
	std::string op1;
	std::string op2;
	std::string opt;
	std::string res;
	int countBrackets;
};

class Button {
public:
	Button():ppp(NULL) {}
	BOOL Create(
		LPCWSTR   lpWindowName,
		HWND      hWndParent,
		HMENU     hMenu = NULL,
		int       x = 0,
		int       y = 0,
		int       nWidth = 80,
		int       nHeight = 60,
		DWORD     dwStyle = WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		LPVOID    lpParam = NULL
	)
	{
		HWND hResultButton = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			lpWindowName,      // Button text 
			dwStyle,  // Styles 
			x,         // x position 
			y,         // y position 
			nWidth,        // Button width
			nHeight,        // Button height
			hWndParent,     // Parent window
			hMenu,       // No menu.
			(HINSTANCE)GetWindowLongPtr(hWndParent, GWLP_HINSTANCE),	// A handle to the instance of the module to be associated with the window.
			NULL);      // Pointer not needed.
		ppp = hResultButton;
		return hResultButton ? TRUE : FALSE;
	}
	HWND ppp;
};


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	Calculator win;

	// if (!win.Create(L"Learn to Program Windows", WS_OVERLAPPEDWINDOW))
	if (!win.Create(L"计算器", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME))
	{
		return 0;
	}

	ShowWindow(win.Window(), nCmdShow);

	// Run the message loop.

	MSG msg = { };
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(win.Window(), hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}

HWND hOperation;
HWND hResult;

// 使用完记得delete TCHAR*
// 字符串转换为wchar_t
TCHAR* OutText(const std::string& op) {
	wchar_t* t = new wchar_t[op.length() + 1];
	int i = 0;
	while (op[i]) {
		t[i] = op[i];
		i++;
	}
	t[i] = 0;
	return t;
}

// 修改窗口内容
void ChangeWindowText(HWND hwnd, const std::string& temp) {
	TCHAR* t = OutText(temp);
	SetWindowText(hwnd, t);
	delete[] t;
}

// 去掉多余的0或小数点
void filterZero(std::string& str) {

	int i = str.length() - 1;
	while (str[i] == '0') {
		str.pop_back();
		i--;
	}

	if (str[i] == '.') {
		str.pop_back();
	}

}
Button buttona;
LRESULT Calculator::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_CREATE:
	{

		hOperation = CreateWindow(L"static", L"asd", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_RIGHT, 0, 0, W_WIDTH - 15, 40, m_hwnd, NULL, NULL, NULL);
		hResult = CreateWindow(L"static", L"asd", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_RIGHT, 0, 39, W_WIDTH - 15, 40, m_hwnd, NULL, NULL, NULL);

		/*
		* 按钮位置按照从下往上，从左往右计算
		* 因此加多一行只需改y的值
		* 加多一列不需要改x，直接加按钮即可
		*/
		int x = 10, y = 440;

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				int t = 3 * i + j + 1;
				buttona.Create(OutText(std::to_string(t)), m_hwnd, (HMENU)(3300 + t), x + j * 90, y - (4 - i) * 70);
			}
		}

		if (!buttona.Create(L"(", m_hwnd, (HMENU)3316, x, y - 5*70)) {
			return 0;
		}


		if (!buttona.Create(L")", m_hwnd, (HMENU)3317, x + 90, y - 5*70)) {
			return 0;
		}

		if (!buttona.Create(L"Enter", m_hwnd, (HMENU)3310, x, y - 70)) {
			return 0;
		}

		if (!buttona.Create(L"0", m_hwnd, (HMENU)3300, x + 90, y - 70)) {
			return 0;
		}

		if (!buttona.Create(L"+", m_hwnd, (HMENU)3311, x + 2 * 90, y - 70)) {
			return 0;
		}

		if (!buttona.Create(L"-", m_hwnd, (HMENU)3312, x + 3 * 90, y - 70)) {
			return 0;
		}

		if (!buttona.Create(L"*", m_hwnd, (HMENU)3313, x + 3 * 90, y - 2 * 70)) {
			return 0;
		}

		if (!buttona.Create(L"/", m_hwnd, (HMENU)3314, x + 3 * 90, y - 3 * 70)) {
			return 0;
		}

		if (!buttona.Create(L"C", m_hwnd, (HMENU)3315, x + 3 * 90, y - 4 * 70)) {
			return 0;
		}

		SetWindowText(hOperation, L"");
		SetWindowText(hResult, L"0");

		return 0;
	}

	case WM_COMMAND:
	{
		SetFocus(m_hwnd);
		int cmd = LOWORD(wParam);
		
		// 数字处理
		if (3300 <= cmd && cmd <= 3309 || cmd == 3316) {

			// 吐了opt[0] = '+' size为0，要opt[0] = "+"才正常
			// 因为原本opt[0] = '\0',直接赋值opt[0] = '+'会把'\0'覆盖,本来opt.size()没有改变，而且字符串末尾是未知的，不是'\0'
			if (!opt.empty()) {
				// 运算符后按数字，运算符落实到计算式中
				op2 += opt;
				opt.clear();
			}

			if (cmd == 3316) {
				op2 += "(";
				countBrackets++;
				ChangeWindowText(hOperation, op2);
				return 0;
			}

			if (opt.empty() && op1 == res) {
				// 如果运算符为空 && 输入数为上次的结果数则重置
				res.clear();
				op1.clear();
			}

			if (op1[0] == '0') {
				op1.clear();
			}

			op1 += char(cmd - 3300 + '0');

			ChangeWindowText(hResult, op1);
		}
		else {

			if (opt.empty()) {
				op2 += op1;
				op1.clear();
			}

			// 右括号
			if (cmd == 3317 && countBrackets) {
				if (!opt.empty()) {
					opt.clear();
				}
				op2 += ")";
				countBrackets--;
				ChangeWindowText(hOperation, op2);
			}

			// 补齐右括号运算结果
			std::string t(op2);
			if (countBrackets) {
				for (int i = countBrackets; i > 0; i--) {
					t += ")";
				}
			}

			res = calculateSolve(t);
			filterZero(res);
			ChangeWindowText(hResult, res);

			switch (cmd) {
			case 3310: {
				// 显示结果
				ChangeWindowText(hOperation, t);
				op2.clear();
				op1 = res;
				opt.clear();
				countBrackets = 0;
				return 0;
			}
			case 3311: {
				opt = "+";
				ChangeWindowText(hOperation, op2 + opt);
				return 0;
			}

			case 3312: {
				opt = "-";
				ChangeWindowText(hOperation, op2 + opt);
				return 0;
			}

			case 3313: {
				opt = "*";
				ChangeWindowText(hOperation, op2 + opt);
				return 0;
			}

			case 3314: {
				opt = "/";
				ChangeWindowText(hOperation, op2 + opt);
				return 0;
			}
			}
		}

		if (cmd == 3315) {
			op1 = "0";
			op2.clear();
			res.clear();
			opt.clear();
			countBrackets = 0;
			SetWindowText(hOperation, L"");
			SetWindowText(hResult, L"0");
			return 0;
		}

		return 0;
	}

	case WM_KEYUP: {
		int cmd = LOWORD(wParam);
		// 数字键
		if (0x30 <= cmd && cmd <= 0x39) {
			SendMessage(m_hwnd, WM_COMMAND, 3300 + (cmd - 0x30), 0);
			return 0;
		}

		// 小键盘数字键
		if (0x60 <= cmd && cmd <= 0x69) {
			//MessageBox(m_hwnd, L"you", L"f", MB_ICONINFORMATION);
			SendMessage(m_hwnd, WM_COMMAND, 3300 + (cmd - 0x60), 0);
			return 0;
		}

		// 操作符
		// 回车，加减乘除
		switch (cmd) {
		case VK_RETURN: {
			SendMessage(m_hwnd, WM_COMMAND, 3310, 0);
			return 0;
		}
		case VK_ADD: {
			SendMessage(m_hwnd, WM_COMMAND, 3311, 0);
			return 0;
		}
		case VK_SUBTRACT: {
			SendMessage(m_hwnd, WM_COMMAND, 3312, 0);
			return 0;
		}
		case VK_MULTIPLY: {
			SendMessage(m_hwnd, WM_COMMAND, 3313, 0);
			return 0;
		}
		case VK_DIVIDE: {
			SendMessage(m_hwnd, WM_COMMAND, 3314, 0);
			return 0;
		}
		default:
			return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
		}
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(m_hwnd, &ps);
		return 0;
	}

	default:
		return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

BOOL Calculator::Create(
	PCWSTR lpWindowName,
	DWORD dwStyle,
	DWORD dwExStyle,
	int x,
	int y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	HMENU hMenu
)
{
	WNDCLASS wc = { 0 };

	wc.lpfnWndProc = MainWindow::WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = ClassName();

	RegisterClass(&wc);

	m_hwnd = CreateWindowEx(
		dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
		nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
	);

	return (m_hwnd ? TRUE : FALSE);
}