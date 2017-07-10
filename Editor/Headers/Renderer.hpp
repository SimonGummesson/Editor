#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <chrono>
#include <ctime>

#include "ColorPass.hpp"
#include "Camera.hpp"

enum inputCommand { idle, walkForward, walkBackward, strafeRight, strafeLeft };

class Renderer
{
private:
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* backBufferRTV;
	int width;
	int height;
	float clearColor[4];

	ColorPass *colorPass;
	Camera* camera;
	std::string command;

	inputCommand getInput();
	void consumeInput(inputCommand command, float dt);

	std::chrono::time_point<std::chrono::system_clock> lastTime;
public:
	Renderer(HWND wndHandle, int width, int height);
	void drawFrame();
	void update();
	void setColorPass(ColorPass *colorPass);
	IDXGISwapChain* getSwapChain();
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	ID3D11RenderTargetView* getBackBufferRTV();
	~Renderer();
};