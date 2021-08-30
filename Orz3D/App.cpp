#include "App.h"
#include <sstream>
#include <iomanip>

#include "Drawable/Box.h"
#include <memory>

App::App() : wnd(800, 600, "app")
{ }


App::App(int width, int height, const char* name) 
	: wnd(width, height, name)
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 20; i++)
	{
		boxes.push_back(std::make_unique<Box>(wnd.Gfx(), rng, adist, ddist, odist, rdist));
	}
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}


App::~App()
{ }


void App::doFrame()
{
	const float t = timer.Mark();
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
	
	wnd.Gfx().ClearBuffer(0.07f, 0.0f, 0.12f);

	for (auto& b : boxes)
	{
		b->Update(t);
		b->Draw(wnd.Gfx());
	}

	//wnd.Gfx().DrawTestTriangle(t, wnd.mouse.GetPosX()/400.0f - 1.0f, -wnd.mouse.GetPosY() / 300.0f + 1.0f);
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