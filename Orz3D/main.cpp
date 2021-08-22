#include "window.h"


// 需要_In_检测, 删除时会有C28251：“WinMain"批注不一致的警告出现
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,          // handle to current instance
	_In_opt_ HINSTANCE hPrevInstance,  // handle to previous instance
	_In_ LPSTR lpCmdLine,              // command line
	_In_ int nCmdShow                  // show state
)
{
	try 
	{
		Window window{ 800, 600, "Orz.3D" };
		MSG msg{ 0 };

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		return static_cast<int>(msg.wParam);
	}
	catch (const BzException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e) 
	{
		MessageBox(nullptr, e.what(), "std::exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}
