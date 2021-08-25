#include "App.h"
#include <sstream>
#include <iomanip>

App::App() : wnd(800, 600, "app")
{ }


App::App(int width, int height, const char* name) 
	: wnd(width, height, name)
{ }


App::~App()
{ }


void App::doFrame()
{
	const float t = timer.Peek();
	/*std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(3) << std::fixed << t << "s";
	wnd.SetTitle(oss.str());*/
	wnd.Gfx().ClearBuffer(sin(t), cos(1 - t), 1);

	wnd.Gfx().EndFrame();
}


int App::Run()
{
	MSG msg{ 0 };

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			doFrame();
	}
	return static_cast<int>(msg.wParam);
}