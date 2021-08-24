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
};

