#pragma once
#include "../Headers/Renderer.hpp"
#include <chrono>
#include <ctime>

class Editor
{
public:
	float fps;
	void update();
	Renderer* getRenderer();
	Editor(HWND &wndHandle, int width, int height, float fps);
	~Editor();
private:
	std::chrono::time_point<std::chrono::system_clock> lastTime;
	Renderer* renderer;
	float elapsedTime;
};