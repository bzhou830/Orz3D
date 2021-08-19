#pragma once
#include <windows.h>

class Window
{
private:
    class WindowClass
    {
    public:
        static const char* GetName() noexcept;
        static HINSTANCE GetInstance() noexcept;
    private:
        WindowClass() noexcept;
        ~WindowClass();
        WindowClass(const WindowClass&) = delete;
        WindowClass& operator=(const WindowClass&) = delete;
        static constexpr const char* wndClassName = "Orz3D Engine Window";
        static WindowClass wndClass;
        HINSTANCE hInst;
    };
public:
    Window(int width, int height, const char* name) noexcept;
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
private:
	static LRESULT CALLBACK WndProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    int width;
    int height;
    HWND hWnd;
};

