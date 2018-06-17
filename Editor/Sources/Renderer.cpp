#include "../Headers/Renderer.hpp"
#include <iostream>
void Renderer::getInput(float dt)
{
	inputs.WKey = GetAsyncKeyState('W');
	inputs.AKey = GetAsyncKeyState('A');
	inputs.SKey = GetAsyncKeyState('S');
	inputs.DKey = GetAsyncKeyState('D');
	inputs.ShiftKey = GetAsyncKeyState(VK_SHIFT);
	inputs.SpaceKey = GetAsyncKeyState(VK_SPACE);
	inputs.LCTRLKey = GetAsyncKeyState(VK_CONTROL);
	inputs.RMouse = GetAsyncKeyState(VK_RBUTTON);
}

Renderer::Renderer(HWND& wndHandle, float width, float height)
{
	this->width = width;
	this->height = height;
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = wndHandle;                           // the window to be used
	scd.SampleDesc.Count = 1;                               // how many multisamples
	scd.SampleDesc.Quality = 0;
	scd.BufferDesc.Width = (UINT)width;
	scd.BufferDesc.Height = (UINT)height;
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

															// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&device,
		NULL,
		&deviceContext);

	if (SUCCEEDED(hr))
	{
		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		device->CreateRenderTargetView(pBackBuffer, NULL, &backBufferRTV);
		pBackBuffer->Release();

		//Create texture for depth stencil
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
		descDepth.Width = (UINT)width;
		descDepth.Height = (UINT)height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		ID3D11Texture2D *pTexture = NULL;
		device->CreateTexture2D(&descDepth, NULL, &pTexture);


		// Create the depth stencil view
		hr = device->CreateDepthStencilView(pTexture,	// Depth stencil texture
			NULL,										// Depth stencil desc
			&depthStencilView);							// [out] Depth stencil view

		// Bind the depth stencil view
		deviceContext->OMSetRenderTargets(1,	        // One rendertarget view
			&backBufferRTV,								// Render target view, created earlier
			depthStencilView);							// Depth stencil view for the render target


		// set the render target as the back buffer
		deviceContext->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
		pTexture->Release();
	}

	clearColor[0] = 0.f;
	clearColor[1] = 0.f;
	clearColor[2] = 0.05f;
	clearColor[3] = 1.f;

	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = 0.f;
	vp.TopLeftY = 0.f;
	deviceContext->RSSetViewports(1, &vp);

	soundManager = new SoundManager();
}

void Renderer::drawFrame()
{
	deviceContext->ClearRenderTargetView(backBufferRTV, clearColor);
	
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);

	for (size_t i = 0; i < passes.size(); i++)
		passes[i]->drawPass();

	swapChain->Present(0, 0);
}

void Renderer::update(float dt)
{
	getInput(dt);
	camera->update(inputs, dt);
	if (inputs.WKey || inputs.AKey || inputs.SKey || inputs.DKey)
		soundManager->playSound("Footsteps");
	else
		soundManager->stopSound("Footsteps");

	soundManager->update(camera->getViewMatrix());
	for (size_t i = 0; i < passes.size(); i++)
		passes[i]->update(dt);
}

void Renderer::addPass(Pass * pass)
{
	passes.push_back(pass);
}

void Renderer::setCamera(Camera * camera)
{
	this->camera = camera;
	soundManager->setPlayerPosPointer(camera->getPositionPointer());
}

SoundManager * Renderer::getSoundManager()
{
	return soundManager;
}

IDXGISwapChain * Renderer::getSwapChain()
{
	return swapChain;
}

ID3D11Device * Renderer::getDevice()
{
	return device;
}

ID3D11DeviceContext * Renderer::getDeviceContext()
{
	return deviceContext;
}

ID3D11RenderTargetView * Renderer::getBackBufferRTV()
{
	return backBufferRTV;
}

Renderer::~Renderer()
{
	for (size_t i = 0; i < passes.size(); i++)
		delete passes[i];

	delete camera;
	delete soundManager;

	backBufferRTV->Release();
	swapChain->Release();
	device->Release();
	deviceContext->Release();
	depthStencilView->Release();
}
