#include "window.h"
#include "WindowsMessageMap.h"
#include "resource.h"


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


Window::Window(int width, int height, const char* name) noexcept
{
    RECT wr{ 0 };
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
	// �������client rect�Ĵ�С������rect��С����Ҫ������
    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
    // create window & get hWnd
	//CreateWindow�е����һ������ָ����this, ����ں����п���ͨ����ȡ�����������
    hWnd = CreateWindow(
        WindowClass::GetName(), name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, WindowClass::GetInstance(), this);
	
    // show window
    ShowWindow(hWnd, SW_SHOWDEFAULT);
}


Window::~Window()
{
    DestroyWindow(hWnd);
}


LRESULT CALLBACK Window::WndProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	if (msg == WM_NCCREATE)
	{
		// ��ȡCreateWindowʱ����Ĳ����������lParam���Ǵ��������Ľṹ��������ṹ�е�lpCreateParams��ȡ������ʱ�����this.
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
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return S_OK;
}
