#include "../Headers/Object.hpp"

Object::Object()
{
	this->vertexBuffer = nullptr;
	this->indexBuffer = nullptr;
}

Object::~Object()
{
	this->vertexBuffer->Release();
	this->indexBuffer->Release();
}

void Object::setBuffers(ID3D11Device* device, std::vector<Vertex> vertexes, UINT32 offset, UINT32 vertexSize, std::vector<unsigned int> indices)
{
	this->offset = offset;
	this->vertexSize = vertexSize;
	this->indexCount = (unsigned int)indices.size();

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * (UINT)indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices.data();
	HRESULT hr = device->CreateBuffer(&indexBufferDesc, &iinitData, &this->indexBuffer);
	if (FAILED(hr))
		std::cout << "Failed to create index buffer!" << std::endl;
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = 2 * sizeof(XMFLOAT3) * (UINT)vertexes.size();

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertexes.data();
	hr = device->CreateBuffer(&bufferDesc, &data, &this->vertexBuffer);
	if (FAILED(hr))
		std::cout << "Failed to create vertex buffer!" << std::endl;
}

void Object::draw(ID3D11DeviceContext &deviceContext)
{
	deviceContext.IASetVertexBuffers(0, 1, &this->vertexBuffer, &this->vertexSize, &this->offset);
	deviceContext.IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext.DrawIndexed(indexCount, 0, 0);
}
