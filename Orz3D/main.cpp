#include "BzWin.h"
#include "window.h"


// ��Ҫ_In_���, ɾ��ʱ����C28251����WinMain"��ע��һ�µľ������
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
