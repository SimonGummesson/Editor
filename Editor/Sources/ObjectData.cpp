#include "..\Headers\ObjectData.hpp"

ObjectData::ObjectData(std::string name, ID3D11Device* device, std::vector<Vertex> vertexes, std::vector<unsigned int> indices, D3D_PRIMITIVE_TOPOLOGY primitiveTopology)
{
	this->name = name;
	this->primitiveTopology = primitiveTopology;

	this->indexCount = (unsigned int)indices.size();
	this->vertexCount = (unsigned int)vertexes.size();

	HRESULT hr;
	if (this->indexCount != 0)
	{
		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned int) * (UINT)indices.size();
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = indices.data();
		hr = device->CreateBuffer(&indexBufferDesc, &iinitData, &this->indexBuffer);
		if (FAILED(hr))
			std::cout << "Failed to create index buffer!" << std::endl;
	}
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = 2 * sizeof(DirectX::XMFLOAT3) * (UINT)vertexes.size();

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertexes.data();
	hr = device->CreateBuffer(&bufferDesc, &data, &this->vertexBuffer);
	if (FAILED(hr))
		std::cout << "Failed to create vertex buffer!" << std::endl;
}

std::string ObjectData::getName()
{
	return this->name;
}

ID3D11Buffer ** ObjectData::getVertexBuffer()
{
	return &this->vertexBuffer;
}

ID3D11Buffer ** ObjectData::getIndexBuffer()
{
	return &this->indexBuffer;
}

unsigned int ObjectData::getVertexCount()
{
	return this->vertexCount;
}

unsigned int ObjectData::getIndexCount()
{
	return this->indexCount;
}

D3D_PRIMITIVE_TOPOLOGY ObjectData::getPrimitiveTopology()
{
	return this->primitiveTopology;
}

void ObjectData::addOject(Object * object)
{
	this->objects.push_back(object);
}

void ObjectData::Draw(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX & VPMatrix, ID3D11Buffer* cbuffer)
{
	for (unsigned int i = 0; i < this->objects.size(); i++)
	{
		this->updateBuffer(deviceContext, objects[i]->getWorldMatrix() * VPMatrix, cbuffer);
		if (this->indexCount == 0)
		{
			deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
			deviceContext->Draw(this->vertexCount, 0);
		}
		else
		{
			deviceContext->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			deviceContext->DrawIndexed(this->indexCount, 0, 0);
		}
	}
}

void ObjectData::updateBuffer(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX & worldMatrix, ID3D11Buffer* cbuffer)
{
	HRESULT hr;
	//	Disable GPU access to the constant buffer data.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = deviceContext->Map(cbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
		std::cout << "Failed to disable gpu access to constant buffer." << std::endl;
	//	Update the constant buffer here.
	VS_COLORPASS_CONSTANT_BUFFER* dataptr = (VS_COLORPASS_CONSTANT_BUFFER*)mappedResource.pData;
	dataptr->WVPMatrix = XMMatrixTranspose(worldMatrix);
	//	Reenable GPU access to the constant buffer data.
	deviceContext->Unmap(cbuffer, 0);
}

ObjectData::~ObjectData()
{
	this->vertexBuffer->Release();
	if (this->indexCount != 0)
		this->indexBuffer->Release();
}
