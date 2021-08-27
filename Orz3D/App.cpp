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
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	static bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);

	/*std::pair<int, int> pos = wnd.mouse.GetPos();
	std::ostringstream oss;*/
	
	//oss << "Time elapsed: " << std::setprecision(3) << std::fixed << t << "s";
	//oss << "Pos x: " << pos.first << " y: " << pos.second << std::endl;
	//wnd.SetTitle(oss.str());
	
	wnd.Gfx().ClearBuffer(sin(t), cos(1 - t), 1);

	wnd.Gfx().DrawTestTriangle(t, wnd.mouse.GetPosX()/400.0f - 1.0f, -wnd.mouse.GetPosY() / 300.0f + 1.0f);
	// 画两个cube的时候可以观察是否存在远处的物体画出来了，但是近处的物体没有画出来的情况
	//wnd.Gfx().DrawTestTriangle(t, 0.1f, 0.1f);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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