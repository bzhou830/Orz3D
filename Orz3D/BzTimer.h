#pragma once
#include <chrono>

class BzTimer
{
public:
	BzTimer();
	float Mark();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point last;
};

