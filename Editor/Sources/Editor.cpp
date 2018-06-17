#include "../Headers/Editor.hpp"

void Editor::update()
{
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();	// Get current time
	std::chrono::duration<float> deltaTime = now - lastTime;									// Calculate difference
	lastTime = now;																				// Update old time

	float dt = deltaTime.count();
	elapsedTime += dt;
	if (elapsedTime >= fps)
	{	
		//cout << 1 / elapsedTime << endl;
		renderer->update(elapsedTime);
		renderer->drawFrame();
		elapsedTime = 0.f;
	}
}

Renderer * const Editor::getRenderer()
{
	return renderer;
}

void Editor::setRendererCamera(Camera * camera)
{
	renderer->setCamera(camera);
}

Editor::Editor(HWND &wndHandle, float width, float height, float fps)
{
	lastTime = std::chrono::system_clock::now();
	renderer = new Renderer(wndHandle, width, height);
	this->fps = 1.f / fps;
	elapsedTime = 0.f;
}

Editor::~Editor()
{
	delete renderer;
}
