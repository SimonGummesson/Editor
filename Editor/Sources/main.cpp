#include <windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "../Headers/Editor.hpp"
#include "../Headers/ColorPass.hpp"
#include "../Headers/Object.hpp"
#include "../Headers/TestObject.hpp"
#include "../Headers/structs.hpp"
#include "../Headers/HeightMap.hpp"


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
	
	if (wndHandle)
	{
		Editor editor(wndHandle, 1280.f, 1024.f, 60.f); // width, height, fps
		// Create standard pass
		ColorPass *colorPass = new ColorPass(editor.getRenderer()->getDevice());
		colorPass->setVertexShaderAndLayout(editor.getRenderer()->getDevice(), L"Shaders/colorVertexShader.hlsl");
		colorPass->setVertexSizeAndOffset(sizeof(Vertex), 0);
		colorPass->setPixelShader(editor.getRenderer()->getDevice(), L"Shaders/colorPixelShader.hlsl");
		colorPass->setGeometryShader(editor.getRenderer()->getDevice(), L"Shaders/colorGeometryShader.hlsl");

		std::vector<Vertex> vertexes;
		vertexes.push_back(Vertex({ -0.5f, -0.5f, 3.0f }, { 1.0f, 0.0f, 0.0f }));
		vertexes.push_back(Vertex({ -0.5f, 0.5f, 3.0f }, { 0.0f, 1.0f, 0.0f }));
		vertexes.push_back(Vertex({ 0.5f, -0.5f, 3.0f }, { 0.0f, 1.0f, 0.0f }));

		std::vector<unsigned int> indices;
		ObjectData *quad = new ObjectData("quad", editor.getRenderer()->getDevice(), vertexes, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Object *quadObject = new TestObject("quad");
		Object *quadObject2 = new TestObject("quad");
		
		quadObject->translate({ 0.f, 0.f, 0.f });
		quadObject->scale({ 10.5f, 10.5f, 0.f });
		quadObject->rotate({ 0.f ,0.f , 3.14159265359f * 3.f / 2.f });
		quadObject->updateWorldMatrix();
		quadObject2->translate({ 0.f, 0.f, 0.f });
		quadObject2->scale({ 10.5f, 10.5f, 0.f });
		quadObject2->rotate({0.f, 0.f, 3.14159265359f / 2.f });
		quadObject2->updateWorldMatrix();


		std::vector<Vertex> heightMapVertexes;
		heightMapVertexes.push_back(Vertex({ -0.5f, -20.f,  -0.5f }, { 0.f, 1.f, 0.f }));
		heightMapVertexes.push_back(Vertex({ -0.5f, -20.f,  0.5f }, { 0.f, 1.f, 0.f }));
		heightMapVertexes.push_back(Vertex({  0.5f, -20.f,  -0.5f }, { 0.f, 1.f, 0.f }));
		heightMapVertexes.push_back(Vertex({  0.5f, -20.f,  0.5f }, { 0.f, 1.f, 0.f }));

		ObjectData *heightMapData = new ObjectData("heightMap", editor.getRenderer()->getDevice(), heightMapVertexes, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		Object *HeightMapObject = new HeightMap("heightMap");

		HeightMapObject->scale({ 10.f, 0.f, 10.f});
		HeightMapObject->updateWorldMatrix();

		colorPass->addObjectData(quad);
		colorPass->addObject(quadObject);
		colorPass->addObject(quadObject2);

		colorPass->addObjectData(heightMapData);
		colorPass->addObject(HeightMapObject);
		
		editor.getRenderer()->setColorPass(colorPass);
		ShowWindow(wndHandle, nCmdShow);

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				editor.update();
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