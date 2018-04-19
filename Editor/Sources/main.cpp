#include <windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#include "../Headers/Editor.hpp"
#include "../Headers/ColorPass.hpp"
#include "../Headers/texturePass.hpp"
#include "../Headers/SkyBoxPass.hpp"
#include "../Headers/Object.hpp"
#include "../Headers/TestObject.hpp"
#include "../Headers/structs.hpp"
#include "../Headers/HeightMap.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../Headers/stb_image.h"

HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void initiateheightMap(string filename, std::vector<VertexColor>& heightMapVertexes, std::vector<unsigned int>& heightMapIndices, float spacingX, float spacingZ, float heightScaling, int& heightmapWidth, int& heightmapHeight);

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

		// Create standard pass for color objects
		ColorPass* colorPass = new ColorPass(editor.getRenderer()->getDevice(), editor.getRenderer()->getDeviceContext(), camera->getWPMatrixPointer(), camera->getPositionPointer());
		D3D11_INPUT_ELEMENT_DESC colorInputDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		colorPass->setVertexShaderAndLayout(editor.getRenderer()->getDevice(), colorInputDesc, ARRAYSIZE(colorInputDesc),L"Shaders/Color pass/Color Shaders/colorVertexShader.hlsl");
		colorPass->setVertexSizeAndOffset(sizeof(VertexColor), 0);
		colorPass->setPixelShader(editor.getRenderer()->getDevice(), L"Shaders/Color pass/Color Shaders/colorPixelShader.hlsl");
		colorPass->setGeometryShader(editor.getRenderer()->getDevice(), L"Shaders/Color pass/Color Shaders/colorGeometryShader.hlsl");

		// Create standard pass for texture objects
		TexturePass* texturePass = new TexturePass(editor.getRenderer()->getDevice(), editor.getRenderer()->getDeviceContext(), camera->getWPMatrixPointer(), camera->getPositionPointer());
		D3D11_INPUT_ELEMENT_DESC textureInputDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		texturePass->setVertexShaderAndLayout(editor.getRenderer()->getDevice(), textureInputDesc, ARRAYSIZE(textureInputDesc), L"Shaders/Color pass/Texture Shaders/textureVertexShader.hlsl");
		texturePass->setVertexSizeAndOffset(sizeof(VertexUV), 0);
		texturePass->setPixelShader(editor.getRenderer()->getDevice(), L"Shaders/Color pass/Texture Shaders/texturePixelShader.hlsl");

		Light light = Light(POINT_LIGHT);
		light.attA = 0.f;
		light.attB = 0.01f;
		light.maxDist = 5.f;
		light.diffuseColor = float4(1.f, 1.f, 1.f, 1.f);
		light.specularColor = float4(1.f, 1.f, 1.f, 1.f);
		light.position = float4(0.f, 10.f, 0.f, 0.f);

		Light light2 = Light(POINT_LIGHT);
		light2.attA = 0.f;
		light2.attB = 0.01f;
		light2.maxDist = 5.f;
		light2.diffuseColor = float4(1.f, 0.f, 0.f, 1.f);
		light2.specularColor = float4(1.f, 0.f, 0.f, 1.f);
		light2.position = float4(10.f, 5.f, 0.f, 0.f);

		Light light3 = Light(POINT_LIGHT);
		light3.attA = 0.f;
		light3.attB = 0.01f;
		light3.maxDist = 5.f;
		light3.diffuseColor = float4(0.f, 0.f, 1.f, 1.f);
		light3.specularColor = float4(0.f, 0.f, 1.f, 1.f);
		light3.position = float4(-10.f, 5.f, 0.f, 0.f);

		Light light4 = Light(DIRECTIONAL_LIGHT);
		light4.diffuseColor = float4(0.05f, 0.05f, 0.05f, 1.f);
		light4.specularColor = float4(0.05f, 0.05f, 0.05f, 1.f);
		light4.position = float4(-1.f, -1.f, 0.f, 0.f);

		colorPass->addLight(light, "Point Light");
		colorPass->addLight(light2, "Red Point Light");
		colorPass->addLight(light3, "Blue Point Light");
		colorPass->addLight(light4, "Sun");
		colorPass->setAmbientLight(Vector3(0.05f, 0.05f, 0.05f));
		colorPass->updateLightBuffer(editor.getRenderer()->getDeviceContext());

		texturePass->addLight(light, "Point Light");
		texturePass->addLight(light2, "Red Point Light");
		texturePass->addLight(light3, "Blue Point Light");
		texturePass->addLight(light4, "Sun");
		texturePass->setAmbientLight(Vector3(0.05f, 0.05f, 0.05f));
		texturePass->updateLightBuffer(editor.getRenderer()->getDeviceContext());

		std::vector<VertexColor> vertexes;
		vertexes.push_back(VertexColor({ -0.5f, -0.5f, 3.0f }, { 1.0f, 0.0f, 0.0f }));
		vertexes.push_back(VertexColor({ -0.5f, 0.5f, 3.0f }, { 0.0f, 1.0f, 0.0f }));
		vertexes.push_back(VertexColor({ 0.5f, -0.5f, 3.0f }, { 0.0f, 1.0f, 0.0f }));

		std::vector<unsigned int> indices;
		ObjectData *quad = new ObjectData("quad", editor.getRenderer()->getDevice(), vertexes, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Object *quadObject = new TestObject("quad");
		Object *quadObject2 = new TestObject("quad");
		
		quadObject->setTranslation({ 0.f, 20.f, 0.f });
		quadObject->scale({ 10.5f, 10.5f, 0.f });
		quadObject->rotate({ 0.f, 0.f, 3.14159265359f * 3.f / 2.f });
		quadObject->updateWorldMatrix();
		quadObject2->setTranslation({ 0.f, 20.f, 0.f });
		quadObject2->scale({ 10.5f, 10.5f, 0.f });
		quadObject2->rotate({0.f, 0.f, 3.14159265359f / 2.f });
		quadObject2->updateWorldMatrix();


		std::vector<VertexColor> heightMapVertexes;
		std::vector<unsigned int> heightMapIndices;
		int heightMapWidth, heightMapHeight;
		float spacingX = 1.f;
		float spacingZ = 1.f;
		initiateheightMap("Resources/heightmap.png", heightMapVertexes, heightMapIndices, spacingX, spacingZ, 5.f, heightMapWidth, heightMapHeight);

		ObjectData *heightMapData = new ObjectData("heightMap", editor.getRenderer()->getDevice(), heightMapVertexes, heightMapIndices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		heightMapData->lightData.ambientColor = Vector3(0.3f, 0.3f, 0.3f);
		HeightMap *HeightMapObject = new HeightMap("heightMap", heightMapVertexes, Vector2(1.f, 1.f));
		HeightMapObject->setQuadDimensions(spacingX, spacingZ);
		HeightMapObject->setDimensions(heightMapWidth, heightMapHeight);
		HeightMapObject->scale({ 0.5f, 0.5f, 0.5f});
		HeightMapObject->translate({ -50.f, 0.f, -50.f });
		/*HeightMapObject->rotate({ 0.f, 3.14f, 0.f });*/
		HeightMapObject->updateWorldMatrix();

		camera->setHeightMap(HeightMapObject);
		ObjectData *box = new ObjectData("box", "Resources/Crate.obj",editor.getRenderer()->getDevice());
		Object *boxObject = new TestObject("box");

		SkyBoxPass* skyBoxPass = new SkyBoxPass(editor.getRenderer()->getDevice(), editor.getRenderer()->getDeviceContext(), 1.f, camera->getWPMatrixPointer(), camera->getPositionPointer());
		skyBoxPass->setCubeMap("Resources/skybox", ".png");
		D3D11_INPUT_ELEMENT_DESC skyboxInputDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		skyBoxPass->setVertexShaderAndLayout(editor.getRenderer()->getDevice(), skyboxInputDesc, ARRAYSIZE(skyboxInputDesc), L"Shaders/Sky box pass/skyBoxVertexShader.hlsl");
		skyBoxPass->setVertexSizeAndOffset(sizeof(VertexUV), 0);
		skyBoxPass->setPixelShader(editor.getRenderer()->getDevice(), L"Shaders/Sky box pass/skyBoxPixelShader.hlsl");

		boxObject->setTranslation({0.f, 5.f, 0.f});
		texturePass->addObjectData(box);
		texturePass->addObject(boxObject);

		colorPass->addObjectData(quad);
		colorPass->addObject(quadObject);
		colorPass->addObject(quadObject2);

		colorPass->addObjectData(heightMapData);
		colorPass->addObject(HeightMapObject);
		
		editor.getRenderer()->addPass(colorPass);
		editor.getRenderer()->addPass(texturePass);
		editor.getRenderer()->addPass(skyBoxPass);
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

void initiateheightMap(string filename, std::vector<VertexColor>& heightMapVertexes, std::vector<unsigned int>& heightMapIndices, float spacingX, float spacingZ, float heightScaling, int& heightmapWidth, int& heightmapHeight)
{
	int bpp;
	float* rgb = stbi_loadf(filename.c_str(), &heightmapWidth, &heightmapHeight, &bpp, STBI_grey);
	
	//Build vertex data
	for (int z = 0; z < heightmapHeight; z++)
	{
		for (int x = 0; x < heightmapWidth; x++)
		{
			VertexColor v0;

			v0.position = Vector3(x * spacingX, rgb[z * heightmapWidth + x] * heightScaling, z * spacingZ);
			v0.color = Vector3(0.25f, 1.f, 0.25f);

			heightMapVertexes.push_back(v0);
		}
	}
	free(rgb);

	//Build index buffer data
	for (int z = 0; z < heightmapHeight - 1; z++)
	{
		for (int x = 0; x < heightmapWidth; x++)
		{
			heightMapIndices.push_back(z * heightmapWidth + x);
			heightMapIndices.push_back((z + 1) * heightmapWidth + x);
		}

		heightMapIndices.push_back(heightMapIndices[heightMapIndices.size() - 1]);
		heightMapIndices.push_back((z + 2) * heightmapWidth);
	}
}