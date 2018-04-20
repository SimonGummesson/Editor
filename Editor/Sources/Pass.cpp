#include "../Headers/Pass.hpp"

Pass::Pass()
{
	deviceContext = nullptr;

	vertexSize = 0;
	vertexOffset = 0;
	vertexLayout = nullptr;
	vertexShader = nullptr;

	pixelShader = nullptr;
}

Pass::~Pass()
{
	if (vertexLayout != nullptr)
		vertexLayout->Release();
	if (vertexShader != nullptr)
		vertexShader->Release();
	if (pixelShader != nullptr)
		pixelShader->Release();
}

void Pass::setVertexShaderAndLayout(ID3D11Device * device, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT numInputElements, LPCWSTR path)
{
	// Create vertex shader
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* pVS = nullptr;
	HRESULT hr = D3DCompileFromFile(
		path,           // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pVS,			// double pointer to ID3DBlob		
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

	hr = device->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &vertexShader);

	// Create input layout
	/*D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};*/
	hr = device->CreateInputLayout(inputElementDesc, numInputElements, pVS->GetBufferPointer(), pVS->GetBufferSize(), &vertexLayout);
	if (FAILED(hr))
		std::cout << "Failed to create vertex shader input layout for shader: " << path << std::endl;
	// we do not need anymore this COM object, so we release it.
	pVS->Release();
}

void Pass::setVertexSizeAndOffset(UINT32 vertexSize, UINT32 offset)
{
	this->vertexSize = vertexSize;
	this->vertexOffset = offset;
}

//void Pass::setGeometryShader(ID3D11Device * device, LPCWSTR path)
//{
//	//create geometry shader
//	ID3DBlob* errorBlob = nullptr;
//	ID3DBlob* pGS = nullptr;
//	HRESULT hr = D3DCompileFromFile(
//		path,           // filename
//		nullptr,		// optional macros
//		nullptr,		// optional include files
//		"GS_main",		// entry point
//		"gs_5_0",		// shader model (target)
//		0,				// shader compile options
//		0,				// effect compile options
//		&pGS,			// double pointer to ID3DBlob		
//		&errorBlob		// pointer for Error Blob messages.
//						// how to use the Error blob, see here
//						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
//	);
//	if (FAILED(hr))
//	{
//		if (errorBlob != nullptr)
//		{
//			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
//			errorBlob->Release();
//		}
//	}
//	device->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &geometryShader);
//
//	pGS->Release();
//}

void Pass::setPixelShader(ID3D11Device * device, LPCWSTR path)
{
	//create pixel shader
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* pPS = nullptr;
	HRESULT hr = D3DCompileFromFile(
		path,           // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
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

	device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &pixelShader);

	pPS->Release();
}

void Pass::setDeviceContext(ID3D11DeviceContext * deviceContext)
{
	this->deviceContext = deviceContext;
}
