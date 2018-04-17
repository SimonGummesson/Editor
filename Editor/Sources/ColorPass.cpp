#include "../Headers/ColorPass.hpp"

void ColorPass::drawPass()
{
	// bind light
	deviceContext->PSSetShaderResources(0, 1, &lightSRV);

	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->GSSetShader(geometryShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);

	deviceContext->IASetInputLayout(vertexLayout);

	deviceContext->GSSetConstantBuffers(0, 1, &GSConstantBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &PSConstantBuffer);
	deviceContext->PSSetConstantBuffers(1, 1, &PSLightDataConstantBuffer);

	this->updatePSBuffer(deviceContext, *cameraPos);

	for (unsigned int i = 0; i < objectData.size(); i++)
	{
		updatePSLightBuffer(deviceContext, objectData[i]->lightData);
		deviceContext->IASetVertexBuffers(0, 1, objectData[i]->getVertexBuffer(), &vertexSize, &vertexOffset);
		deviceContext->IASetPrimitiveTopology(objectData[i]->getPrimitiveTopology());
		objectData[i]->Draw(deviceContext, *cameraVPMatrix, this->GSConstantBuffer);
	}
}

void ColorPass::update(float dt)
{
	for (unsigned int i = 0; i < objects.size(); i++)
		objects[i]->update(dt);
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

void ColorPass::updatePSBuffer(ID3D11DeviceContext * deviceContext, Vector3 cameraPos)
{
	//	Disable GPU access to the constant buffer data.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = deviceContext->Map(this->PSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
		cout << "Failed to disable gpu access to constant buffer." << endl;
	//	Update the constant buffer here.
	Vector4* dataptr = (Vector4*)mappedResource.pData;
	*dataptr = Vector4(cameraPos.x, cameraPos.y, cameraPos.z, 1.f);
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
	for (size_t i = 0; i < lights.size(); i++)
		if (light == lights[i])
			lights.erase(lights.begin() + i);
}

ColorPass::ColorPass(ID3D11Device * device, ID3D11DeviceContext* deviceContext, Matrix* cameraVPMatrix, Vector3* cameraPos)
{
	this->deviceContext = deviceContext;
	this->cameraVPMatrix = cameraVPMatrix;
	this->cameraPos = cameraPos;

	// Fill in a buffer description.
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(WVP_BUFFER);
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
	PScbDesc.ByteWidth = sizeof(Vector4);
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

	// Light raw buffer creation
	D3D11_BUFFER_DESC lightBufferDesc;
	ZeroMemory(&lightBufferDesc, sizeof(lightBufferDesc));
	lightBufferDesc.ByteWidth = 100 * sizeof(Light) + sizeof(int) * 8; // Default buffer supports 100 lights
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

	for (unsigned int i = 0; i < objectData.size(); i++)
		delete objectData[i];

	this->GSConstantBuffer->Release();
	this->PSConstantBuffer->Release();
	this->PSLightDataConstantBuffer->Release();
	
	this->lightBuffer->Release();
	this->lightSRV->Release();
}

void ColorPass::setVPPointer(Matrix * cameraVPMatrix)
{
	this->cameraVPMatrix = cameraVPMatrix;
}

void ColorPass::setCameraPosPointer(Vector3 * cameraPos)
{
	this->cameraPos = cameraPos;
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
	lightInformation[0].y = 0;
	lightInformation[0].z = 0;
	lightInformation[0].w = 0;

	lightInformation[1].x = *reinterpret_cast<UINT*> (&ambientLight.x);
	lightInformation[1].y = *reinterpret_cast<UINT*> (&ambientLight.y);
	lightInformation[1].z = *reinterpret_cast<UINT*> (&ambientLight.z);
	lightInformation[1].w = 0;
	for (size_t i = 0; i < lights.size(); i++)
	{
		if (POINT_LIGHT == lights[i].type)
			lightInformation[2 + i * 4].x = 0;
		else if (DIRECTIONAL_LIGHT == lights[i].type)
			lightInformation[2 + i * 4].x = 1;
		lightInformation[2 + i * 4].y = *reinterpret_cast<UINT*> (&lights[i].attA);
		lightInformation[2 + i * 4].z = *reinterpret_cast<UINT*> (&lights[i].attB);
		lightInformation[2 + i * 4].w = *reinterpret_cast<UINT*> (&lights[i].maxDist);

		lightInformation[2 + i * 4 + 1].x = *reinterpret_cast<UINT*> (&lights[i].diffuseColor.x);
		lightInformation[2 + i * 4 + 1].y = *reinterpret_cast<UINT*> (&lights[i].diffuseColor.y);
		lightInformation[2 + i * 4 + 1].z = *reinterpret_cast<UINT*> (&lights[i].diffuseColor.z);
		lightInformation[2 + i * 4 + 1].w = 0;

		lightInformation[2 + i * 4 + 2].x = *reinterpret_cast<UINT*> (&lights[i].specularColor.x);
		lightInformation[2 + i * 4 + 2].y = *reinterpret_cast<UINT*> (&lights[i].specularColor.y);
		lightInformation[2 + i * 4 + 2].z = *reinterpret_cast<UINT*> (&lights[i].specularColor.z);
		lightInformation[2 + i * 4 + 2].w = 0;

		if (POINT_LIGHT == lights[i].type)
		{
			lightInformation[2 + i * 4 + 3].x = *reinterpret_cast<UINT*> (&lights[i].position.x);
			lightInformation[2 + i * 4 + 3].y = *reinterpret_cast<UINT*> (&lights[i].position.y);
			lightInformation[2 + i * 4 + 3].z = *reinterpret_cast<UINT*> (&lights[i].position.z);
			lightInformation[2 + i * 4 + 3].w = 0;
		}
		else if (DIRECTIONAL_LIGHT == lights[i].type)
		{
			lightInformation[2 + i * 4 + 3].x = *reinterpret_cast<UINT*> (&lights[i].direction.x);
			lightInformation[2 + i * 4 + 3].y = *reinterpret_cast<UINT*> (&lights[i].direction.y);
			lightInformation[2 + i * 4 + 3].z = *reinterpret_cast<UINT*> (&lights[i].direction.z);
			lightInformation[2 + i * 4 + 3].w = 0;
		}
	}

	//upload to GPU
	byte4UInt* dataptr = (byte4UInt*)mappedResource.pData;
	memcpy(dataptr, lightInformation, bufferSize);
	for (size_t i = 0; i < 4 * lights.size() + 2; i++)
		cout << "( " << dataptr[i].x << ", " << dataptr[i].y << ", " << dataptr[i].z << ", " << dataptr[i].w << ")" << endl;
	//	Reenable GPU access to the constant buffer data.
	deviceContext->Unmap(this->lightBuffer, 0);
	free(lightInformation);
}
