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
	for (auto i = 0; i < 5; i++)
	{
		TexBoxes.push_back(std::make_unique<TexBox>(wnd.Gfx(), rng, adist, ddist, odist, rdist));
	}
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	wnd.Gfx().SetCamera(dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f));
}


App::~App()
{ }


void App::doFrame()
{
	const float t = timer.Mark() * speed_factor;
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);

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

	// imgui window to control simulation speed
	if (ImGui::Begin("Simulation Speed"))
	{
		static char buffer[1024];
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 4.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::InputText("Inputs", buffer, sizeof(buffer));
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
