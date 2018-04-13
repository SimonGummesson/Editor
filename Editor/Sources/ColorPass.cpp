#include "../Headers/ColorPass.hpp"

void ColorPass::drawPass(ID3D11DeviceContext *deviceContext, Matrix VPMatrix, Vector3 cameraPos)
{
	// bind lights
	deviceContext->PSSetShaderResources(0, 1, &lightSRV);
	// draw all objects using color
	deviceContext->VSSetShader(this->colorVertexShader, nullptr, 0);
	deviceContext->GSSetShader(this->colorGeometryShader, nullptr, 0);
	deviceContext->PSSetShader(this->colorPixelShader, nullptr, 0);

	deviceContext->IASetInputLayout(this->colorVertexLayout);

	deviceContext->GSSetConstantBuffers(0, 1, &this->GSConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &this->PSConstantBuffer);
	deviceContext->PSSetConstantBuffers(1, 1, &this->PSLightDataConstantBuffer);

	this->updatePSBuffer(deviceContext, cameraPos);

	for (unsigned int i = 0; i < this->objectDataColor.size(); i++)
	{
		updatePSLightBuffer(deviceContext, this->objectDataColor[i]->lightData);
		deviceContext->IASetVertexBuffers(0, 1, objectDataColor[i]->getVertexBuffer(), &this->colorVertexSize, &this->colorOffset);
		deviceContext->IASetPrimitiveTopology(objectDataColor[i]->getPrimitiveTopology());
		objectDataColor[i]->Draw(deviceContext, VPMatrix, this->GSConstantBuffer);
	}

	//draw all objects using textures
	deviceContext->VSSetShader(this->textureVertexShader, nullptr, 0);
	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->PSSetShader(this->texturePixelShader, nullptr, 0);

	deviceContext->IASetInputLayout(this->textureVertexLayout);

	deviceContext->VSSetConstantBuffers(0, 1, &this->GSConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &this->PSConstantBuffer);
	deviceContext->PSSetConstantBuffers(1, 1, &this->PSLightDataConstantBuffer);

	this->updatePSBuffer(deviceContext, cameraPos);

	for (unsigned int i = 0; i < this->objectDataTexture.size(); i++)
	{
		updatePSLightBuffer(deviceContext, this->objectDataTexture[i]->lightData);
		deviceContext->PSSetShaderResources(1, 1, objectDataTexture[i]->getTextureView());
		deviceContext->IASetVertexBuffers(0, 1, objectDataTexture[i]->getVertexBuffer(), &this->textureVertexSize, &this->textureOffset);
		deviceContext->IASetPrimitiveTopology(objectDataTexture[i]->getPrimitiveTopology());
		objectDataTexture[i]->Draw(deviceContext, VPMatrix, this->GSConstantBuffer);
	}
}

void ColorPass::update(float dt)
{
	for (unsigned int i = 0; i < objects.size(); i++)
		objects[i]->update(dt);
}

void ColorPass::setColorVertexShaderAndLayout(ID3D11Device * device, LPCWSTR path)
{
	//create vertex shader
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		path,           // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_4_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pVS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	HRESULT hr = device->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &this->colorVertexShader);
	if (FAILED(hr))
		std::cout << "Failed to create vertex shader: " << path << std::endl;
	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->colorVertexLayout);
	if (FAILED(hr))
		std::cout << "Failed to create vertex shader input layout for shader: " << path << std::endl;
	// we do not need anymore this COM object, so we release it.
	pVS->Release();
}

void ColorPass::setColorVertexSizeAndOffset(UINT32 vertexSize, UINT32 offset)
{
	this->colorVertexSize = vertexSize;
	this->colorOffset = offset;
}

void ColorPass::setColorPixelShader(ID3D11Device * device, LPCWSTR path)
{
	//create pixel shader
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* pPS = nullptr;
	HRESULT hr = D3DCompileFromFile(
		path,           // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_4_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	
	if (errorBlob != nullptr)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		errorBlob->Release();
	}

	device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &this->colorPixelShader);

	pPS->Release();
}

void ColorPass::setColorGeometryShader(ID3D11Device * device, LPCWSTR path)
{
	//create geometry shader
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* pGS = nullptr;
	HRESULT hr = D3DCompileFromFile(
		path,           // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"GS_main",		// entry point
		"gs_4_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pGS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);
	if (FAILED(hr))
	{
		if (errorBlob != nullptr)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
	}
	device->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &this->colorGeometryShader);
	
	pGS->Release();
}

void ColorPass::setTextureVertexShaderAndLayout(ID3D11Device * device, LPCWSTR path)
{
	//create vertex shader
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		path,           // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_4_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pVS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	if (errorBlob != nullptr)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		errorBlob->Release();
	}

	HRESULT hr = device->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &this->textureVertexShader);
	if (FAILED(hr))
		std::cout << "Failed to create vertex shader: " << path << std::endl;
	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->textureVertexLayout);
	if (FAILED(hr))
		std::cout << "Failed to create vertex shader input layout for shader: " << path << std::endl;
	// we do not need anymore this COM object, so we release it.
	pVS->Release();
}

void ColorPass::setTextureVertexSizeAndOffset(UINT32 vertexSize, UINT32 offset)
{
	this->textureVertexSize = vertexSize;
	this->textureOffset = offset;
}

void ColorPass::setTexturePixelShader(ID3D11Device * device, LPCWSTR path)
{
	//create pixel shader
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* pPS = nullptr;
	HRESULT hr = D3DCompileFromFile(
		path,           // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_4_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		&errorBlob		// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);


	if (errorBlob != nullptr)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		errorBlob->Release();
	}

	device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &this->texturePixelShader);

	pPS->Release();
}

bool ColorPass::addObject(Object *object)
{
	for (unsigned int i = 0; i < this->objectDataColor.size(); i++)
	{
		string name = object->getName();
		if (objectDataColor[i]->getName() == name)
		{
			this->objects.push_back(object);
			this->objectDataColor[i]->addOject(object);
			return true;
		}
	}

	for (unsigned int i = 0; i < this->objectDataTexture.size(); i++)
	{
		string name = object->getName();
		if (objectDataTexture[i]->getName() == name)
		{
			this->objects.push_back(object);
			this->objectDataTexture[i]->addOject(object);
			return true;
		}
	}
	return false;
}

void ColorPass::addObjectData(ObjectData * objectData)
{
	if (objectData->getType()) // got texture
	{
		for (unsigned int i = 0; i < this->objectDataTexture.size(); i++)
		{
			if (this->objectDataTexture[i]->getName() == objectData->getName())
				return;
		}
		this->objectDataTexture.push_back(objectData);
	}
	else
	{
		for (unsigned int i = 0; i < this->objectDataColor.size(); i++)
		{
			if (this->objectDataColor[i]->getName() == objectData->getName())
				return;
		}
		this->objectDataColor.push_back(objectData);
	}
}

void ColorPass::updatePSBuffer(ID3D11DeviceContext * deviceContext, Vector3 cameraPos)
{
	//	Disable GPU access to the constant buffer data.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = deviceContext->Map(this->PSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
		cout << "Failed to disable gpu access to constant buffer." << endl;
	//	Update the constant buffer here.
	PS_COLORPASS_CONSTANT_BUFFER* dataptr = (PS_COLORPASS_CONSTANT_BUFFER*)mappedResource.pData;
	dataptr->eyePos = cameraPos;
	dataptr->roughnessValue = 0.3f;
	dataptr->lightPosition = Vector3( 0.f, 100.f, 0.f );
	dataptr->F0 = 0.8f;
	dataptr->eyeForward = Vector3(1.0f, 1.0f, 1.0f);
	dataptr->k = 0.2f;
	//	Reenable GPU access to the constant buffer data.
	deviceContext->Unmap(this->PSConstantBuffer, 0);
}

void ColorPass::updatePSLightBuffer(ID3D11DeviceContext * deviceContext, materialLightData data)
{
	//	Disable GPU access to the constant buffer data.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = deviceContext->Map(this->PSLightDataConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
		cout << "Failed to disable gpu access to constant buffer." << endl;
	//	Update the constant buffer here.
	materialLightData* dataptr = (materialLightData*)mappedResource.pData;
	dataptr->ambientColor = data.ambientColor;
	dataptr->diffuseColor = data.diffuseColor;
	dataptr->specularColor = data.specularColor;
	dataptr->specularPower = data.specularPower;
	//	Reenable GPU access to the constant buffer data.
	deviceContext->Unmap(this->PSLightDataConstantBuffer, 0);
}

void ColorPass::setAmbientLight(Vector3 ambient)
{
	ambientLight = ambient;
}

void ColorPass::addLight(Light light, string name)
{
	lights.push_back(light);
}

void ColorPass::removeLight(Light light)
{
	for (int i = 0; i < lights.size(); i++)
		if (light == lights[i])
			lights.erase(lights.begin() + i);
}

ColorPass::ColorPass(ID3D11Device * device)
{
	this->colorVertexSize = 6;	// default value
	this->colorOffset = 0;		// default value

	this->colorVertexLayout = nullptr;
	this->colorVertexShader = nullptr;
	this->colorGeometryShader = nullptr;
	this->colorPixelShader = nullptr;
	
	this->textureVertexSize = 8;	// default value
	this->textureOffset = 0;		// default value

	this->colorVertexLayout = nullptr;
	this->colorVertexShader = nullptr;
	this->colorGeometryShader = nullptr;
	this->colorPixelShader = nullptr;

	// Fill in a buffer description.
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(GS_COLORPASS_CONSTANT_BUFFER);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Create the buffer.
	HRESULT hr = device->CreateBuffer(&cbDesc, NULL, &this->GSConstantBuffer);

	if (FAILED(hr))
		std::cout << "Failed to create geometry shader constant buffer for color pass" << std::endl;

	//Pixel Shader buffer
	D3D11_BUFFER_DESC PScbDesc;
	PScbDesc.ByteWidth = sizeof(PS_COLORPASS_CONSTANT_BUFFER);
	PScbDesc.Usage = D3D11_USAGE_DYNAMIC;
	PScbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	PScbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	PScbDesc.MiscFlags = 0;
	PScbDesc.StructureByteStride = 0;

	// Create the buffer.
	hr = device->CreateBuffer(&PScbDesc, NULL, &this->PSConstantBuffer);

	if (FAILED(hr))
		std::cout << "Failed to create pixel shader constant buffer for color pass" << std::endl;

	PScbDesc.ByteWidth = sizeof(materialLightData);

	// Create the buffer.
	hr = device->CreateBuffer(&PScbDesc, NULL, &this->PSLightDataConstantBuffer);

	if (FAILED(hr))
		std::cout << "Failed to create pixel shader constant buffer for light data" << std::endl;

	D3D11_BUFFER_DESC lightBufferDesc;
	ZeroMemory(&lightBufferDesc, sizeof(lightBufferDesc));
	lightBufferDesc.ByteWidth = 100 * sizeof(Light) + sizeof(int) * 4;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	lightBufferDesc.StructureByteStride = 0;

	hr = device->CreateBuffer(&lightBufferDesc, nullptr, &lightBuffer);
	
	D3D11_BUFFEREX_SRV buffer;
	buffer.FirstElement = 0;
	buffer.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
	buffer.NumElements = lightBufferDesc.ByteWidth / 4;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.BufferEx = buffer;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;

	hr = device->CreateShaderResourceView(lightBuffer, &srvDesc, &lightSRV);
}

ColorPass::~ColorPass()
{
	for (unsigned int i = 0; i < objects.size(); i++)
		delete objects[i];

	for (unsigned int i = 0; i < objectDataColor.size(); i++)
		delete objectDataColor[i];

	for (unsigned int i = 0; i < objectDataTexture.size(); i++)
		delete objectDataTexture[i];

	this->colorVertexLayout->Release();
	this->colorVertexShader->Release();
	this->colorGeometryShader->Release();
	this->colorPixelShader->Release();

	this->textureVertexLayout->Release();
	this->textureVertexShader->Release();
	this->texturePixelShader->Release();

	this->GSConstantBuffer->Release();
	this->PSConstantBuffer->Release();
	this->PSLightDataConstantBuffer->Release();
	
	this->lightBuffer->Release();
	this->lightSRV->Release();
}

void ColorPass::updateLightBuffer(ID3D11DeviceContext * deviceContext)
{
	//	Disable GPU access to the constant buffer data.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	
	// Create array with light information
	size_t bufferSize = sizeof(Light) * lights.size() + 2 * sizeof(byte4UInt);
	byte4UInt* lightInformation = (byte4UInt*)malloc(bufferSize);
	lightInformation[0].x = lights.size();
	lightInformation[0].y = 0.f;
	lightInformation[0].z = 0.f;
	lightInformation[0].w = 0.f;

	lightInformation[1].x = *reinterpret_cast<UINT*> (&ambientLight.x);
	lightInformation[1].y = *reinterpret_cast<UINT*> (&ambientLight.y);
	lightInformation[1].z = *reinterpret_cast<UINT*> (&ambientLight.z);
	lightInformation[1].w = 0.f;
	for (int i = 0; i < lights.size(); i++)
	{
		if (POINT_LIGHT == lights[i].type)
			lightInformation[2 + i * 4].x = 0;
		else if (DIRECTIONAL_LIGHT == lights[i].type)
			lightInformation[2 + i * 4].x = 1;
		lightInformation[2 + i * 4].y = *reinterpret_cast<UINT*> (&lights[i].misc1);
		lightInformation[2 + i * 4].z = *reinterpret_cast<UINT*> (&lights[i].misc2);
		lightInformation[2 + i * 4].w = *reinterpret_cast<UINT*> (&lights[i].misc3);

		lightInformation[2 + i * 4 + 1].x = *reinterpret_cast<UINT*> (&lights[i].diffuseColor.x);
		lightInformation[2 + i * 4 + 1].y = *reinterpret_cast<UINT*> (&lights[i].diffuseColor.y);
		lightInformation[2 + i * 4 + 1].z = *reinterpret_cast<UINT*> (&lights[i].diffuseColor.z);
		lightInformation[2 + i * 4 + 1].w = 0.f;

		lightInformation[2 + i * 4 + 2].x = *reinterpret_cast<UINT*> (&lights[i].specularColor.x);
		lightInformation[2 + i * 4 + 2].y = *reinterpret_cast<UINT*> (&lights[i].specularColor.y);
		lightInformation[2 + i * 4 + 2].z = *reinterpret_cast<UINT*> (&lights[i].specularColor.z);
		lightInformation[2 + i * 4 + 2].w = 0.f;

		if (POINT_LIGHT == lights[i].type)
		{
			lightInformation[2 + i * 4 + 3].x = *reinterpret_cast<UINT*> (&lights[i].position.x);
			lightInformation[2 + i * 4 + 3].y = *reinterpret_cast<UINT*> (&lights[i].position.y);
			lightInformation[2 + i * 4 + 3].z = *reinterpret_cast<UINT*> (&lights[i].position.z);
			lightInformation[2 + i * 4 + 3].w = 0.f;
		}
		else if (DIRECTIONAL_LIGHT == lights[i].type)
		{
			lightInformation[2 + i * 4 + 3].x = *reinterpret_cast<UINT*> (&lights[i].direction.x);
			lightInformation[2 + i * 4 + 3].y = *reinterpret_cast<UINT*> (&lights[i].direction.y);
			lightInformation[2 + i * 4 + 3].z = *reinterpret_cast<UINT*> (&lights[i].direction.z);
			lightInformation[2 + i * 4 + 3].w = 0.f;
		}
	}

	//upload to GPU
	byte4UInt* dataptr = (byte4UInt*)mappedResource.pData;
	memcpy(dataptr, lightInformation, bufferSize);
	for (int i = 0; i < 4 * lights.size() + 2; i++)
		cout << "( " << dataptr[i].x << ", " << dataptr[i].y << ", " << dataptr[i].z << ", " << dataptr[i].w << ")" << endl;
	//	Reenable GPU access to the constant buffer data.
	deviceContext->Unmap(this->lightBuffer, 0);
	free(lightInformation);
}
