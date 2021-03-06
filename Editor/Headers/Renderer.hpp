#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <chrono>
#include <ctime>

#include "Pass.hpp"
#include "ColorPass.hpp"
#include "texturePass.hpp"
#include "Camera.hpp"
#include "SimpleMath.h"
#include "SoundManager.hpp"

using namespace DirectX::SimpleMath;

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

	vector<Pass*> passes;
	Camera* camera;
	SoundManager* soundManager;
	std::string command;

	void getInput(float dt);

	//input variables
	InputStatus inputs;
public:
	Renderer(HWND& wndHandle, float width, float height);
	void drawFrame();
	void update(float dt);
	void addPass(Pass *colorPass);
	void setCamera(Camera* camera);
	SoundManager* const getSoundManager();
	IDXGISwapChain* const getSwapChain();
	ID3D11Device* const getDevice();
	ID3D11DeviceContext* const getDeviceContext();
	ID3D11RenderTargetView* const getBackBufferRTV();
	~Renderer();
};