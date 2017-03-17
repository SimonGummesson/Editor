#include "../Headers/ColorPass.hpp"

void ColorPass::drawPass(ID3D11DeviceContext &deviceContext)
{
	deviceContext.VSSetShader(this->vertexShader, nullptr, 0);
	deviceContext.HSSetShader(this->hullShader, nullptr, 0);
	deviceContext.DSSetShader(this->domainShader, nullptr, 0);
	deviceContext.GSSetShader(this->geometryShader, nullptr, 0);
	deviceContext.PSSetShader(this->pixelShader, nullptr, 0);

	deviceContext.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext.IASetInputLayout(this->vertexLayout);

	for (unsigned int i = 0; i < this->objects.size(); i++)
		this->objects[i]->draw(deviceContext);
}

void ColorPass::setVertexShaderAndLayout(ID3D11Device & device, LPCWSTR path)
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

	HRESULT hr = device.CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &this->vertexShader);
	if (FAILED(hr))
		std::cout << "Failed to create vertex shader: " << path << std::endl;
	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = device.CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->vertexLayout);
	if (FAILED(hr))
		std::cout << "Failed to create vertex shader input layout for shader: " << path << std::endl;
	// we do not need anymore this COM object, so we release it.
	pVS->Release();
}

void ColorPass::setPixelShader(ID3D11Device & device, LPCWSTR path)
{
	//create pixel shader
	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(
		path,           // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_4_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	HRESULT hr = device.CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &this->pixelShader);
	if (FAILED(hr))
		std::cout << "Failed to create pixel shader: " << path << std::endl;
	// we do not need anymore this COM object, so we release it.
	pPS->Release();
}

void ColorPass::addObject(Object *object)
{
	this->objects.push_back(object);
}

ColorPass::ColorPass()
{
	for (unsigned int i = 0; i < objects.size(); i++)
		objects.erase(objects.begin() + i);
	this->vertexLayout = nullptr;
	this->vertexShader = nullptr;
	this->hullShader = nullptr;
	this->domainShader = nullptr;
	this->geometryShader = nullptr;
	this->pixelShader = nullptr;
}

ColorPass::~ColorPass()
{
	for (unsigned int i = 0; i < objects.size(); i++)
		delete objects[i];
	this->vertexLayout->Release();
	this->vertexShader->Release();

	if (this->hullShader != nullptr)
		this->hullShader->Release();
	
	if (this->domainShader != nullptr)
		this->domainShader->Release();
	
	if (this->geometryShader != nullptr)
		this->geometryShader->Release();
	
	if (this->pixelShader != nullptr)
		this->pixelShader->Release();
}
