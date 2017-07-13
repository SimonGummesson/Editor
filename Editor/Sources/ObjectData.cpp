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

ObjectData::~ObjectData()
{
	this->vertexBuffer->Release();
	if (this->indexCount != 0)
		this->indexBuffer->Release();
}
