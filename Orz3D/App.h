#pragma once
#include "window.h"
#include "BzTimer.h"

class App
{
public:
	App();
	App(int width, int height, const char* name);
	~App();
	int Run();
private:
	void doFrame();
	Window wnd;
	BzTimer timer;
	float speed_factor = 0.3f;
	std::vector<std::unique_ptr<class Box>> boxes;
	std::vector<std::unique_ptr<class TexBox>> TexBoxes;
};

