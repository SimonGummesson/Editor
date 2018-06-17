#pragma once
#include "../Headers/Renderer.hpp"
#include <chrono>
#include <ctime>
#include "Camera.hpp"
class Editor
{
public:
	float fps;
	void update();
	Renderer* const getRenderer();
	void setRendererCamera(Camera* camera);
	Editor(HWND &wndHandle, float width, float height, float fps);
	~Editor();
private:
	std::chrono::time_point<std::chrono::system_clock> lastTime;
	Renderer* renderer;
	float elapsedTime;
};