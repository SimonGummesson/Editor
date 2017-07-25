#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <chrono>
#include <ctime>

#include "ColorPass.hpp"
#include "Camera.hpp"
#include "SimpleMath.h"
#include "SimpleMath.inl"

using namespace DirectX;

class Renderer
{
private:
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* depthStencilView;
	float width;
	float height;
	float clearColor[4];

	ColorPass *colorPass;
	Camera* camera;
	std::string command;

	void getInput(float dt);
	POINT lastCursorPosition;
public:
	Renderer(HWND& wndHandle, float width, float height);
	void drawFrame();
	void update(float dt);
	void setColorPass(ColorPass *colorPass);
	IDXGISwapChain* getSwapChain();
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	ID3D11RenderTargetView* getBackBufferRTV();
	~Renderer();
};