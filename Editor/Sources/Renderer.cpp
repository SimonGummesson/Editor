#include "../Headers/Renderer.hpp"
#include <iostream>
void Renderer::getInput(float dt)
{
	SHORT WKey = GetAsyncKeyState('W');
	SHORT AKey = GetAsyncKeyState('A');
	SHORT SKey = GetAsyncKeyState('S');
	SHORT DKey = GetAsyncKeyState('D');
	SHORT ShiftKey = GetAsyncKeyState(VK_SHIFT);
	SHORT SpaceKey = GetAsyncKeyState(VK_SPACE);
	SHORT LCTRLKey = GetAsyncKeyState(VK_CONTROL);
	SHORT LMouse = GetAsyncKeyState(VK_LBUTTON);
	SHORT RKey = GetAsyncKeyState('R');
	
	bool running = false;
	
	if (ShiftKey)
		running = true;
	if (WKey)
		this->camera->moveCamera(this->camera->getForward() * dt, running);
	if (AKey)
		this->camera->moveCamera(-this->camera->getRight() * dt, running);
	if (SKey)
		this->camera->moveCamera(-this->camera->getForward() * dt, running);
	if (DKey)
		this->camera->moveCamera(this->camera->getRight() * dt, running);
	if (SpaceKey)
		this->camera->moveCamera({0, dt, 0}, running);
	if (LCTRLKey)
		this->camera->moveCamera({ 0, -dt, 0 }, running);

	if (LMouse)
	{
		ShowCursor(FALSE);
		POINT currentMousePos;
		GetCursorPos(&currentMousePos);

		//Horizontal camera rotation
		float deltaX = (float)currentMousePos.x - (float)this->lastCursorPosition.x;
		if (deltaX != 0.f)
		{
			float sign = signbit(deltaX) ? -1.0f : 1.0f;
			Matrix rot = DirectX::XMMatrixRotationY(sign * this->camera->rotationSpeed * dt);
			this->camera->setRight(Vector3::Transform(this->camera->getRight(), rot));
			this->camera->setForward(Vector3::Transform(this->camera->getForward(), rot));
			this->camera->setUp(Vector3::Transform(this->camera->getUp(), rot));
		}

		//Vertical camera rotation
		float deltaY = (float)currentMousePos.y - (float)this->lastCursorPosition.y;
		if (deltaY != 0.f)
		{
			float sign = signbit(deltaY) ? -1.0f : 1.0f;
			Matrix rot = DirectX::XMMatrixRotationAxis(this->camera->getRight(), sign * this->camera->rotationSpeed * dt);
			this->camera->setUp(Vector3::Transform(this->camera->getUp(), rot));
			this->camera->setForward(Vector3::Transform(this->camera->getForward(), rot));
			this->camera->setRight(Vector3::Transform(this->camera->getRight(), rot));
		}
		this->lastCursorPosition = currentMousePos;
	}
	else
		ShowCursor(TRUE);

	this->camera->setViewMatrix(DirectX::XMMatrixLookAtLH(this->camera->getPosition(), this->camera->getPosition() + this->camera->getForward(), this->camera->getUp()));

	if (RKey)
	{
		cout << ">> ";
		string command;
		getline(std::cin, command);
		cout << command << endl;
		GetCursorPos(&this->lastCursorPosition);
	}
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

		//Create texture for depth stencil
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));
		descDepth.Width = (UINT)width;
		descDepth.Height = (UINT)height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 4;
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
			&this->backBufferRTV,						// Render target view, created earlier
			depthStencilView);							// Depth stencil view for the render target


		// set the render target as the back buffer
		this->deviceContext->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
		pTexture->Release();
	}

	this->clearColor[0] = 0.f;
	this->clearColor[1] = 0.f;
	this->clearColor[2] = 0.05f;
	this->clearColor[3] = 1.f;

	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	this->deviceContext->RSSetViewports(1, &vp);

	this->camera = new Camera(this->width, this->height, 1.75f, 10.f, 20.f);
}

void Renderer::drawFrame()
{
	this->deviceContext->ClearRenderTargetView(this->backBufferRTV, this->clearColor);
	
	this->deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	this->colorPass->drawPass(this->deviceContext, this->camera->getVPMatrix(), this->camera->getPosition());

	this->swapChain->Present(0, 0);
}

void Renderer::update(float dt)
{
	this->getInput(dt);
	this->colorPass->update(dt);
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
	delete this->camera;
	
	this->backBufferRTV->Release();
	this->swapChain->Release();
	this->device->Release();
	this->deviceContext->Release();
	this->depthStencilView->Release();
}
