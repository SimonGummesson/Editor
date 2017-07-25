#include "../Headers/ColorPass.hpp"

void ColorPass::drawPass(ID3D11DeviceContext *deviceContext, DirectX::XMMATRIX& VPMatrix, DirectX::XMVECTOR cameraPos)
{
	deviceContext->VSSetShader(this->vertexShader, nullptr, 0);
	deviceContext->HSSetShader(this->hullShader, nullptr, 0);
	deviceContext->DSSetShader(this->domainShader, nullptr, 0);
	deviceContext->GSSetShader(this->geometryShader, nullptr, 0);
	deviceContext->PSSetShader(this->pixelShader, nullptr, 0);

	deviceContext->IASetInputLayout(this->vertexLayout);

	deviceContext->GSSetConstantBuffers(0, 1, &this->GSConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &this->PSConstantBuffer);
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, cameraPos);
	this->updatePSBuffer(deviceContext, pos);

	for (unsigned int i = 0; i < this->objectData.size(); i++)
	{
		deviceContext->IASetVertexBuffers(0, 1, objectData[i]->getVertexBuffer(), &this->vertexSize, &this->offset);
		deviceContext->IASetPrimitiveTopology(objectData[i]->getPrimitiveTopology());
		objectData[i]->Draw(deviceContext, VPMatrix, this->GSConstantBuffer);
	}
}

void ColorPass::update(float dt)
{
	for (unsigned int i = 0; i < objects.size(); i++)
		objects[i]->update(dt);
}

void ColorPass::setVertexShaderAndLayout(ID3D11Device * device, LPCWSTR path)
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

	HRESULT hr = device->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &this->vertexShader);
	if (FAILED(hr))
		std::cout << "Failed to create vertex shader: " << path << std::endl;
	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->vertexLayout);
	if (FAILED(hr))
		std::cout << "Failed to create vertex shader input layout for shader: " << path << std::endl;
	// we do not need anymore this COM object, so we release it.
	pVS->Release();
}

void ColorPass::setVertexSizeAndOffset(UINT32 vertexSize, UINT32 offset)
{
	this->vertexSize = vertexSize;
	this->offset = offset;
}

void ColorPass::setPixelShader(ID3D11Device * device, LPCWSTR path)
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

	device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &this->pixelShader);

	pPS->Release();
}

void ColorPass::setGeometryShader(ID3D11Device * device, LPCWSTR path)
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
	device->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &this->geometryShader);
	
	pGS->Release();
}

bool ColorPass::addObject(Object *object)
{
	for (unsigned int i = 0; i < this->objectData.size(); i++)
	{
		string name = object->getName();
		if (objectData[i]->getName() == name)
		{
			this->objects.push_back(object);
			this->objectData[i]->addOject(object);
			return true;
		}
	}
	return false;
}

void ColorPass::addObjectData(ObjectData * objectData)
{
	for (unsigned int i = 0; i < this->objectData.size(); i++)
	{
		if (this->objectData[i]->getName() == objectData->getName())
			return;
	}
	this->objectData.push_back(objectData);
}

void ColorPass::updatePSBuffer(ID3D11DeviceContext * deviceContext, XMFLOAT3 cameraPos)
{
	//	Disable GPU access to the constant buffer data.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = deviceContext->Map(this->PSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
		cout << "Failed to disable gpu access to constant buffer." << endl;
	//	Update the constant buffer here.
	PS_COLORPASS_CONSTANT_BUFFER* dataptr = (PS_COLORPASS_CONSTANT_BUFFER*)mappedResource.pData;
	dataptr->eyePos = cameraPos,
	dataptr->F0 = 0.8f;
	dataptr->k = 0.2f;
	dataptr->lightPosition = { 0.f, 100.f, 0.f };
	dataptr->roughnessValue = 0.3f;
	//	Reenable GPU access to the constant buffer data.
	deviceContext->Unmap(this->PSConstantBuffer, 0);
}

ColorPass::ColorPass(ID3D11Device * device)
{
	this->vertexSize = 6;	// default value
	this->offset = 0;		// default value

	this->vertexLayout = nullptr;
	this->vertexShader = nullptr;
	this->hullShader = nullptr;
	this->domainShader = nullptr;
	this->geometryShader = nullptr;
	this->pixelShader = nullptr;
	
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

}

ColorPass::~ColorPass()
{
	for (unsigned int i = 0; i < objects.size(); i++)
		delete objects[i];

	for (unsigned int i = 0; i < objectData.size(); i++)
		delete objectData[i];

	this->vertexLayout->Release();
	this->vertexShader->Release();

	this->GSConstantBuffer->Release();
	this->PSConstantBuffer->Release();

	if (this->hullShader != nullptr)
		this->hullShader->Release();
	
	if (this->domainShader != nullptr)
		this->domainShader->Release();
	
	if (this->geometryShader != nullptr)
		this->geometryShader->Release();
	
	if (this->pixelShader != nullptr)
		this->pixelShader->Release();
}
