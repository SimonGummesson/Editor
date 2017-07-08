#include "../Headers/Renderer.hpp"

Renderer::Renderer(HWND wndHandle, int width, int height)
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
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

															// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&this->swapChain,
		&this->device,
		NULL,
		&this->deviceContext);

	if (SUCCEEDED(hr))
	{
		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		this->device->CreateRenderTargetView(pBackBuffer, NULL, &this->backBufferRTV);
		pBackBuffer->Release();

		// set the render target as the back buffer
		this->deviceContext->OMSetRenderTargets(1, &backBufferRTV, NULL);
	}

	this->clearColor[0] = 0;
	this->clearColor[1] = 0;
	this->clearColor[2] = 0;
	this->clearColor[3] = 1;

	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	this->deviceContext->RSSetViewports(1, &vp);
}

void Renderer::drawFrame()
{
	this->deviceContext->ClearRenderTargetView(this->backBufferRTV, this->clearColor);
	
	this->colorPass->drawPass(this->deviceContext);

	this->swapChain->Present(0, 0);
}

void Renderer::setColorPass(ColorPass * colorPass)
{
	this->colorPass = colorPass;
}

IDXGISwapChain * Renderer::getSwapChain()
{
	return this->swapChain;
}

ID3D11Device * Renderer::getDevice()
{
	return this->device;
}

ID3D11DeviceContext * Renderer::getDeviceContext()
{
	return this->deviceContext;
}

ID3D11RenderTargetView * Renderer::getBackBufferRTV()
{
	return this->backBufferRTV;
}

Renderer::~Renderer()
{
	delete this->colorPass;

	this->backBufferRTV->Release();
	this->swapChain->Release();
	this->device->Release();
	this->deviceContext->Release();
}
