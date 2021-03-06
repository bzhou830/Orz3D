#pragma once
#include "BzWin.h"
#include "BzException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <memory>
#include "WindowThrowMacros.h"

class Window
{
public:
	class Exception : public BzException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
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
    Window(int width, int height, const char* name);
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    void SetTitle(const std::string& title);
    Graphics& Gfx();
    Keyboard kbd;
    Mouse mouse;
private:
	static LRESULT CALLBACK WndProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    int width;
    int height;
    HWND hWnd;
	std::unique_ptr<Graphics> pGfx;
};

