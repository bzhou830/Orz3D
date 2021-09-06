#include "App.h"
#include <sstream>
#include <iomanip>

#include "Drawable/Box.h"
#include "Drawable/TexBox.h"
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
	for (auto i = 0; i < 5; i++)
	{
		boxes.push_back(std::make_unique<Box>(wnd.Gfx(), rng, adist, ddist, odist, rdist));
	}
	for (auto i = 0; i < 2; i++)
	{
		TexBoxes.push_back(std::make_unique<TexBox>(wnd.Gfx(), rng, adist, ddist, odist, rdist));
	}
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}


App::~App()
{ }


void App::doFrame()
{
	const float t = timer.Mark() * speed_factor;
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	//static bool show_demo_window = true;
	//ImGui::ShowDemoWindow(&show_demo_window);

	/*std::pair<int, int> pos = wnd.mouse.GetPos();
	std::ostringstream oss;*/
	
	//oss << "Time elapsed: " << std::setprecision(3) << std::fixed << t << "s";
	//oss << "Pos x: " << pos.first << " y: " << pos.second << std::endl;
	//wnd.SetTitle(oss.str());

	for (auto& b : boxes)
	{
		b->Update(t);
		b->Draw(wnd.Gfx());
	}
	for (auto& b : TexBoxes)
	{
		b->Update(t);
		b->Draw(wnd.Gfx());
	}

	//wnd.Gfx().DrawTestTriangle(t, wnd.mouse.GetPosX()/400.0f - 1.0f, -wnd.mouse.GetPosY() / 300.0f + 1.0f);
	// 画两个cube的时候可以观察是否存在远处的物体画出来了，但是近处的物体没有画出来的情况
	//wnd.Gfx().DrawTestTriangle(t, 0.1f, 0.1f);

	

	// imgui window to control simulation speed
	if (ImGui::Begin("Simulation Speed"))
	{
		static char buffer[1024];
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 4.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::InputText("Butts", buffer, sizeof(buffer));
	}
	ImGui::End();

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