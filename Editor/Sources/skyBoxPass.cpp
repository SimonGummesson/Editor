#include "../Headers/SkyBoxPass.hpp"

void SkyBoxPass::drawPass()
{
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);

	deviceContext->IASetInputLayout(vertexLayout);

	skyBoxObjectData->updateBuffer(deviceContext, *WPMatrix, cubeObject->getWorldMatrix(), WVPConstantBuffer);

	deviceContext->VSSetConstantBuffers(0, 1, &WVPConstantBuffer);
	deviceContext->PSSetShaderResources(0, 1, &cubeMapSrv);
	
	deviceContext->IASetVertexBuffers(0, 1, skyBoxObjectData->getVertexBuffer(), &vertexSize, &vertexOffset);
	deviceContext->IASetPrimitiveTopology(skyBoxObjectData->getPrimitiveTopology());

	deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->RSSetState(rasterizerState);
	deviceContext->OMSetDepthStencilState(depthStencilstate, 0);
	deviceContext->PSSetSamplers(0, 1, &skyMapSamplerState);

	deviceContext->Draw(skyBoxObjectData->getVertexCount(), 0);

	deviceContext->PSSetSamplers(0, 0, nullptr);
	deviceContext->RSSetState(nullptr);
	deviceContext->OMSetDepthStencilState(nullptr, 0);
}

void SkyBoxPass::update(float dt)
{
	cubeObject->update(dt);
}

void SkyBoxPass::setCubeMap(string path, string fileExtension)
{
	if (cubeMapSrv != nullptr)
		cubeMapSrv->Release();

	int x, y, spacing;
	D3D11_SUBRESOURCE_DATA pData[6];
	float* imageData[6];
	for (int i = 0; i < 6; i++)
	{
		// 0: top. 1: left, 2: front, 3: right, 4: back, 5: bottom
		string filename = path + to_string(i) + fileExtension;
		imageData[i] = stbi_loadf(filename.c_str(), &x, &y, &spacing, STBI_rgb_alpha);
		pData[i].pSysMem = imageData[i];
		pData[i].SysMemPitch = x * 4 * sizeof(float);
		pData[i].SysMemSlicePitch = 0;
	}

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = x;
	desc.Height = y;
	desc.MipLevels = 1;
	desc.ArraySize = 6;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* texture;
	HRESULT hr = device->CreateTexture2D(&desc, &pData[0], &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = desc.MipLevels;
	srvDesc.TextureCube.MostDetailedMip = 0;

	hr = device->CreateShaderResourceView(texture, &srvDesc, &cubeMapSrv);

	texture->Release();
	for (int i = 0; i < 6; i++)
		free(imageData[i]);
}

SkyBoxPass::SkyBoxPass(ID3D11Device * device, ID3D11DeviceContext* context, float dist, Matrix* WPMatrix, Vector3* cameraPos)
{
	cubeMapSrv = nullptr;
	this->device = device;
	deviceContext = context;
	this->WPMatrix = WPMatrix;
	// build cube with position, UV and normal
	// http://www.directxtutorial.com/Lesson.aspx?lessonid=9-4-7
	vector<VertexUV> vertexes;
	// top
	vertexes.push_back({ -1.f * dist,  1.f * dist, -1.f * dist, 0.f, 1.f,  0.f, -1.f,  0.f }); // 0
	vertexes.push_back({  1.f * dist,  1.f * dist, -1.f * dist, 1.f, 1.f,  0.f, -1.f,  0.f }); // 1
	vertexes.push_back({  1.f * dist,  1.f * dist,  1.f * dist, 1.f, 0.f,  0.f, -1.f,  0.f }); // 5

	vertexes.push_back({ -1.f * dist,  1.f * dist, -1.f * dist, 0.f, 1.f,  0.f, -1.f,  0.f }); // 0
	vertexes.push_back({  1.f * dist,  1.f * dist,  1.f * dist, 1.f, 0.f,  0.f, -1.f,  0.f }); // 5
	vertexes.push_back({ -1.f * dist,  1.f * dist,  1.f * dist, 0.f, 0.f,  0.f, -1.f,  0.f }); // 4

	// left
	vertexes.push_back({ -1.f * dist, -1.f * dist,  1.f * dist, 0.f, 1.f,  1.f,  0.f,  0.f }); // 6
	vertexes.push_back({ -1.f * dist, -1.f * dist, -1.f * dist, 1.f, 1.f,  1.f,  0.f,  0.f }); // 2
	vertexes.push_back({ -1.f * dist,  1.f * dist, -1.f * dist, 1.f, 0.f,  1.f,  0.f,  0.f }); // 0

	vertexes.push_back({ -1.f * dist, -1.f * dist,  1.f * dist, 0.f, 1.f,  1.f,  0.f,  0.f }); // 6
	vertexes.push_back({ -1.f * dist,  1.f * dist, -1.f * dist, 1.f, 0.f,  1.f,  0.f,  0.f }); // 0
	vertexes.push_back({ -1.f * dist,  1.f * dist,  1.f * dist, 0.f, 0.f,  1.f,  0.f,  0.f }); // 4

	// front
	vertexes.push_back({  1.f * dist, -1.f * dist,  1.f * dist, 0.f, 1.f,  0.f,  0.f, -1.f }); // 7
	vertexes.push_back({ -1.f * dist, -1.f * dist,  1.f * dist, 1.f, 1.f,  0.f,  0.f, -1.f }); // 6
	vertexes.push_back({ -1.f * dist,  1.f * dist,  1.f * dist, 1.f, 0.f,  0.f,  0.f, -1.f }); // 4

	vertexes.push_back({  1.f * dist, -1.f * dist,  1.f * dist, 0.f, 1.f,  0.f,  0.f, -1.f }); // 7
	vertexes.push_back({ -1.f * dist,  1.f * dist,  1.f * dist, 1.f, 0.f,  0.f,  0.f, -1.f }); // 4
	vertexes.push_back({  1.f * dist,  1.f * dist,  1.f * dist, 0.f, 0.f,  0.f,  0.f, -1.f }); // 5

	// right
	vertexes.push_back({  1.f * dist, -1.f * dist, -1.f * dist, 0.f, 1.f, -1.f,  0.f,  0.f }); // 3
	vertexes.push_back({  1.f * dist, -1.f * dist,  1.f * dist, 1.f, 1.f, -1.f,  0.f,  0.f }); // 7
	vertexes.push_back({  1.f * dist,  1.f * dist,  1.f * dist, 1.f, 0.f, -1.f,  0.f,  0.f }); // 5

	vertexes.push_back({  1.f * dist, -1.f * dist, -1.f * dist, 0.f, 1.f, -1.f,  0.f,  0.f }); // 3
	vertexes.push_back({  1.f * dist,  1.f * dist,  1.f * dist, 1.f, 0.f, -1.f,  0.f,  0.f }); // 5
	vertexes.push_back({  1.f * dist,  1.f * dist, -1.f * dist, 0.f, 0.f, -1.f,  0.f,  0.f }); // 1

	// back
	vertexes.push_back({ -1.f * dist, -1.f * dist, -1.f * dist, 0.f, 1.f,  0.f,  0.f,  1.f }); // 2
	vertexes.push_back({  1.f * dist, -1.f * dist, -1.f * dist, 1.f, 1.f,  0.f,  0.f,  1.f }); // 3
	vertexes.push_back({  1.f * dist,  1.f * dist, -1.f * dist, 1.f, 0.f,  0.f,  0.f,  1.f }); // 1

	vertexes.push_back({ -1.f * dist, -1.f * dist, -1.f * dist, 0.f, 1.f,  0.f,  0.f,  1.f }); // 2
	vertexes.push_back({  1.f * dist,  1.f * dist, -1.f * dist, 1.f, 0.f,  0.f,  0.f,  1.f }); // 1
	vertexes.push_back({ -1.f * dist,  1.f * dist, -1.f * dist, 0.f, 0.f,  0.f,  0.f, -1.f }); // 0

	// bottom
	vertexes.push_back({ -1.f * dist, -1.f * dist,  1.f * dist, 0.f, 1.f,  0.f,  1.f,  0.f }); // 6
	vertexes.push_back({  1.f * dist, -1.f * dist,  1.f * dist, 1.f, 1.f,  0.f,  1.f,  0.f }); // 7
	vertexes.push_back({  1.f * dist, -1.f * dist, -1.f * dist, 1.f, 0.f,  0.f,  1.f,  0.f }); // 5

	vertexes.push_back({ -1.f * dist, -1.f * dist,  1.f * dist, 0.f, 1.f,  0.f,  1.f,  0.f }); // 6
	vertexes.push_back({  1.f * dist, -1.f * dist, -1.f * dist, 1.f, 0.f,  0.f,  1.f,  0.f }); // 5
	vertexes.push_back({ -1.f * dist, -1.f * dist, -1.f * dist, 0.f, 0.f,  0.f,  1.f,  0.f }); // 4

	std::vector<unsigned int> indices;
	skyBoxObjectData = new ObjectData("skyBox", device, vertexes, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cubeObject = new SkyBoxObject("skyBox", cameraPos);
	// Fill in a buffer description.
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(WVP_BUFFER);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Create the buffer.
	HRESULT hr = device->CreateBuffer(&cbDesc, NULL, &WVPConstantBuffer);

	D3D11_RASTERIZER_DESC rsD;
	rsD.FillMode = D3D11_FILL_SOLID;
	rsD.CullMode = D3D11_CULL_NONE;
	rsD.FrontCounterClockwise = FALSE;
	rsD.DepthBias = D3D11_DEFAULT_DEPTH_BIAS;
	rsD.DepthBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP;
	rsD.SlopeScaledDepthBias = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rsD.DepthClipEnable = TRUE;
	rsD.ScissorEnable = FALSE;
	rsD.MultisampleEnable = FALSE;
	rsD.AntialiasedLineEnable = FALSE;

	hr = device->CreateRasterizerState(&rsD, &rasterizerState);

	D3D11_DEPTH_STENCIL_DESC DSD;
	ZeroMemory(&DSD, sizeof(D3D11_DEPTH_STENCIL_DESC));
	DSD.DepthEnable = true;
	DSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DSD.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = device->CreateDepthStencilState(&DSD, &depthStencilstate);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0.f;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&sampDesc, &skyMapSamplerState);
}

SkyBoxPass::~SkyBoxPass()
{
	cubeMapSrv->Release();
	WVPConstantBuffer->Release();
	rasterizerState->Release();
	depthStencilstate->Release();
	skyMapSamplerState->Release();
	delete skyBoxObjectData;
}
