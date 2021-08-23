#include "App.h"

App::App() : window(800, 600, "app")
{ }


App::App(int width, int height, const char* name) 
	: window(width, height, name)
{ }


App::~App()
{ }


void App::doFrame()
{

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