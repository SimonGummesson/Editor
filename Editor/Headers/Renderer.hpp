#pragma once
#include <windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>

class Renderer
{
private:
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	ID3D11RenderTargetView* backBufferRTV;
public:
	Renderer(HWND wndHandle);
	IDXGISwapChain* getSwapChain();
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	ID3D11RenderTargetView* getBackBufferRTV();
	~Renderer();
};