#include "../Headers/Editor.hpp"

void Editor::update()
{
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();	// Get current time
	std::chrono::duration<float> deltaTime = now - this->lastTime;								// Calculate difference
	this->lastTime = now;																		// Update old time

	float dt = deltaTime.count();
	this->elapsedTime += dt;
	if (this->elapsedTime >= this->fps)
	{	
		this->renderer->update(elapsedTime);
		this->renderer->drawFrame();
		this->elapsedTime = 0.f;
	}
}

Renderer * Editor::getRenderer()
{
	return this->renderer;
}

Editor::Editor(HWND &wndHandle, int width, int height, float fps)
{
	this->lastTime = std::chrono::system_clock::now();
	this->renderer = new Renderer(wndHandle, width, height);
	this->fps = 1.f / fps;
	this->elapsedTime = 0.f;
}

Editor::~Editor()
{
	delete this->renderer;
}
