#include <windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "SDL2.lib")
#include "../Headers/Editor.hpp"
#include "../Headers/ColorPass.hpp"
#include "../Headers/Object.hpp"
#include "../Headers/TestObject.hpp"
#include "../Headers/structs.hpp"
#include "../Headers/HeightMap.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../Headers/stb_image.h"

HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void initiateheightMap(string filename, std::vector<VertexColor>& heightMapVertexes, std::vector<unsigned int>& heightMapIndices, float spacing, float heightScaling);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	CoInitializeEx(0, COINIT_MULTITHREADED); //I need this for WICTextureLoader, don't ask why............
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);

	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hInstance);
	
	if (wndHandle)
	{
		Editor editor(wndHandle, 1280.f, 1024.f, 60.f); // width, height, fps
		Camera* camera = new Camera(1280.f, 1024.f, 1.75f, 10.f, 20.f, 5.f);
		editor.setRendererCamera(camera);
		// Create standard pass
		ColorPass *colorPass = new ColorPass(editor.getRenderer()->getDevice());
		colorPass->setColorVertexShaderAndLayout(editor.getRenderer()->getDevice(), L"Shaders/Color Shaders/colorVertexShader.hlsl");
		colorPass->setColorVertexSizeAndOffset(sizeof(VertexColor), 0);
		colorPass->setColorPixelShader(editor.getRenderer()->getDevice(), L"Shaders/Color Shaders/colorPixelShader.hlsl");
		colorPass->setColorGeometryShader(editor.getRenderer()->getDevice(), L"Shaders/Color Shaders/colorGeometryShader.hlsl");

		colorPass->setTextureVertexShaderAndLayout(editor.getRenderer()->getDevice(), L"Shaders/Texture Shaders/textureVertexShader.hlsl");
		colorPass->setTextureVertexSizeAndOffset(sizeof(VertexUV), 0);
		colorPass->setTexturePixelShader(editor.getRenderer()->getDevice(), L"Shaders/Texture Shaders/texturePixelShader.hlsl");

		std::vector<VertexColor> vertexes;
		vertexes.push_back(VertexColor({ -0.5f, -0.5f, 3.0f }, { 1.0f, 0.0f, 0.0f }));
		vertexes.push_back(VertexColor({ -0.5f, 0.5f, 3.0f }, { 0.0f, 1.0f, 0.0f }));
		vertexes.push_back(VertexColor({ 0.5f, -0.5f, 3.0f }, { 0.0f, 1.0f, 0.0f }));

		std::vector<unsigned int> indices;
		ObjectData *quad = new ObjectData("quad", editor.getRenderer()->getDevice(), vertexes, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Object *quadObject = new TestObject("quad");
		Object *quadObject2 = new TestObject("quad");
		
		quadObject->translate({ 0.f, 0.f, 0.f });
		quadObject->scale({ 10.5f, 10.5f, 0.f });
		quadObject->rotate({ 0.f, 0.f, 3.14159265359f * 3.f / 2.f });
		quadObject->updateWorldMatrix();
		quadObject2->translate({ 0.f, 0.f, 0.f });
		quadObject2->scale({ 10.5f, 10.5f, 0.f });
		quadObject2->rotate({0.f, 0.f, 3.14159265359f / 2.f });
		quadObject2->updateWorldMatrix();


		std::vector<VertexColor> heightMapVertexes;
		std::vector<unsigned int> heightMapIndices;
		initiateheightMap("Resources/heightmap.png", heightMapVertexes, heightMapIndices, 1.f, 5.f);

		ObjectData *heightMapData = new ObjectData("heightMap", editor.getRenderer()->getDevice(), heightMapVertexes, heightMapIndices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		heightMapData->lightData.ambientColor = Vector3(0.3f, 0.3f, 0.3f);
		HeightMap *HeightMapObject = new HeightMap("heightMap", heightMapVertexes, Vector2(1.f, 1.f));
		HeightMapObject->setQuadDimensions(1.f, 1.f);
		HeightMapObject->setDimensions(100, 100);
		HeightMapObject->scale({ 0.5f, 0.5f, 0.5f});
		HeightMapObject->translate({ -50.f, 0.f, -50.f });
		/*HeightMapObject->rotate({ 0.f, 3.14f, 0.f });*/
		HeightMapObject->updateWorldMatrix();

		camera->setHeightMap(HeightMapObject);
		ObjectData *box = new ObjectData("box", "Resources/Crate.obj",editor.getRenderer()->getDevice());
		Object *boxObject = new TestObject("box");

		boxObject->translate({0.f, 5.f, 0.f});
		colorPass->addObjectData(box);
		colorPass->addObject(boxObject);

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

void initiateheightMap(string filename, std::vector<VertexColor>& heightMapVertexes, std::vector<unsigned int>& heightMapIndices, float spacing, float heightScaling)
{
	int bpp;
	int bitmapWidth, bitmapHeight;
	float* rgb = stbi_loadf(filename.c_str(), &bitmapWidth, &bitmapHeight, &bpp, STBI_grey);
	
	//Build vertex data
	for (int z = 0; z < bitmapHeight; z++)
	{
		for (int x = 0; x < bitmapWidth; x++)
		{
			VertexColor v0;

			v0.position = Vector3(x * spacing, rgb[z * bitmapWidth + x] * heightScaling, z * spacing);
			v0.color = Vector3(0.f, 1.f, 0.f);

			heightMapVertexes.push_back(v0);
		}
	}
	free(rgb);

	//Build index buffer data
	for (int z = 0; z < bitmapHeight - 1; z++)
	{
		for (int x = 0; x < bitmapWidth; x++)
		{
			heightMapIndices.push_back(z * bitmapWidth + x);
			heightMapIndices.push_back((z + 1) * bitmapWidth + x);
		}

		heightMapIndices.push_back(heightMapIndices[heightMapIndices.size() - 1]);
		heightMapIndices.push_back((z + 2) * bitmapWidth);
	}
}