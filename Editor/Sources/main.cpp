#include <windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "../Headers/Renderer.hpp"
#include "../Headers/ColorPass.hpp"
#include "../Headers/Object.hpp"
#include "../Headers/structs.hpp"



HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);

	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hInstance);
	
	Renderer renderer(wndHandle, 1280, 1024);
	if (wndHandle)
	{
		// Create standard pass
		ColorPass *colorPass = new ColorPass(renderer.getDevice());
		colorPass->setVertexShaderAndLayout(renderer.getDevice(), L"Shaders/colorVertexShader.hlsl");
		colorPass->setPixelShader(renderer.getDevice(), L"Shaders/colorPixelShader.hlsl");

		Object *triangle = new Object();

		std::vector<Vertex> vertexes;
		vertexes.push_back(Vertex(DirectX::XMFLOAT3(-0.5f,  0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)));
		vertexes.push_back(Vertex(DirectX::XMFLOAT3( 0.5f, -0.5f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)));
		vertexes.push_back(Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)));
		std::vector<unsigned int> indices;

		triangle->setBuffers(renderer.getDevice(), vertexes, 0, sizeof(Vertex), indices);
		
		Object *triangle2 = new Object();
		vertexes[0].position = { -0.5f,  0.5f, 0.f };
		vertexes[1].position = {  0.5f,  0.5f, 0.f };
		vertexes[2].position = {  0.5f, -0.5f, 0.f };
		vertexes[0].color = { 0.f, 0.f, 1.f };
		vertexes[1].color = { 0.f, 1.f, 0.f };
		vertexes[2].color = { 1.f, 0.f, 0.f };
		triangle2->setBuffers(renderer.getDevice(), vertexes, 0, sizeof(Vertex), indices);
		
		triangle->translate(DirectX::XMVECTOR{ -0.2f, 0.f, 0.f });
		triangle2->translate(DirectX::XMVECTOR{ 0.2f, 0.f, 0.f });

		colorPass->addObject(triangle);
		colorPass->addObject(triangle2);
		renderer.setColorPass(colorPass);

		ShowWindow(wndHandle, nCmdShow);

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				renderer.drawFrame();
			}
		}

		DestroyWindow(wndHandle);
	}

	return (int)msg.wParam;
}

HWND InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"Editor";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, 1280, 1024 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND handle = CreateWindow(
		L"Editor",
		L"Editor",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	return handle;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}