#include "window.h"
#include "WindowsMessageMap.h"
#include "resource.h"
#include <sstream>

Window::WindowClass Window::WindowClass::wndClass;


Window::WindowClass::WindowClass() noexcept
    :hInst(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc{ 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProcSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = LoadIcon(GetInstance(), (LPCTSTR)IDI_ICON1);
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = LoadIcon(GetInstance(), (LPCTSTR)IDI_ICON1);
    RegisterClassEx(&wc);
}


Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName, GetInstance());
}


const char* Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}


HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}


Window::Window(int width, int height, const char* name):
	width(width), 
	height(height)
{
    RECT wr{ 0 };
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;

	// 传入的是client rect的大小，窗口rect大小是需要调整的
	if (false == AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE))
		throw CHWND_LAST_EXCEPT();

	//create window & get hWnd. CreateWindow中的最后一个参数指定的this, 这个在后续中可以通过获取创建参数获得
    hWnd = CreateWindow(
        WindowClass::GetName(), name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, WindowClass::GetInstance(), this);
	
	// 创建窗口失败，抛出异常方便调试
	if (hWnd == nullptr)
		throw CHWND_LAST_EXCEPT();
	
    // show window
    ShowWindow(hWnd, SW_SHOWDEFAULT);

	// create graphics object
	pGfx = std::make_unique<Graphics>(hWnd);
}

Graphics& Window::Gfx()
{
	return *pGfx;
}


Window::~Window()
{
    DestroyWindow(hWnd);
}


LRESULT CALLBACK Window::WndProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	if (msg == WM_NCCREATE)
	{
		// 获取CreateWindow时填入的参数，这里的lParam就是创建参数的结构，从这个结构中的lpCreateParams获取到创建时候传入的this.
		const CREATESTRUCTW* const pCreat = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWind = reinterpret_cast<Window*>(pCreat->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWind));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		pWind->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWind = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWind->HandleMsg(hWnd, msg, wParam, lParam);
}


LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//static WindowsMessageMap mm;
	//OutputDebugString(mm(msg, lParam, wParam).c_str());
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	/*********** KEYBOARD MESSAGES ***********/
	case WM_KEYDOWN:
		// syskey commands need to be handled to track ALT key (VK_MENU) and F10
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) // filter autorepeat
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
	{   
		kbd.OnChar(static_cast<unsigned char>(wParam));
		//static std::string str;
		//str += static_cast<char>(wParam);
		//SetWindowText(hWnd, str.c_str());
		break;
	}
	/*********** END KEYBOARD MESSAGES ***********/

	/************* MOUSE MESSAGES ****************/
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// in client region -> log move, and log enter + capture mouse (if not previously in window)
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		// not in client -> log move / maintain capture if button down
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			// button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		// release mouse if outside of window
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		// release mouse if outside of window
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	/************** END MOUSE MESSAGES **************/

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return S_OK;
}

// 后续是 Window Exception 相关代码
Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	: BzException(line, file), hr(hr)
{}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "Bz Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}
