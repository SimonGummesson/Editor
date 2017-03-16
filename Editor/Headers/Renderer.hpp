#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Pass.hpp"

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
	std::vector<Pass*> passes;
public:
	Renderer(HWND wndHandle, int width, int height);
	void drawFrame();
	void addPass(Pass* pass);
	IDXGISwapChain* getSwapChain();
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	ID3D11RenderTargetView* getBackBufferRTV();
	~Renderer();
};