#pragma once
#include "window.h"

class App
{
public:
	App();
	App(int width, int height, const char* name);
	~App();
	int Run();
private:
	void doFrame();
	Window window;
};

