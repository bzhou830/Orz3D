#include "BzWin.h"
#include "window.h"


// 需要_In_检测, 删除时会有C28251：“WinMain"批注不一致的警告出现
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,          // handle to current instance
	_In_opt_ HINSTANCE hPrevInstance,  // handle to previous instance
	_In_ LPSTR lpCmdLine,              // command line
	_In_ int nCmdShow                  // show state
)
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
