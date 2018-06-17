#include "..\Headers\ObjectData.hpp"

ObjectData::ObjectData(string name, ID3D11Device* device, vector<VertexColor> vertexes, std::vector<unsigned int> indices, D3D_PRIMITIVE_TOPOLOGY primitiveTopology)
{
	this->name = name;
	this->primitiveTopology = primitiveTopology;

	indexCount = (unsigned int)indices.size();
	vertexCount = (unsigned int)vertexes.size();

	HRESULT hr;
	if (indexCount != 0)
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
		hr = device->CreateBuffer(&indexBufferDesc, &iinitData, &indexBuffer);
		if (FAILED(hr))
			cout << "Failed to create index buffer!" << endl;
	}
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VertexColor) * (UINT)vertexes.size();

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertexes.data();
	hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	if (FAILED(hr))
		cout << "Failed to create vertex buffer!" << endl;
}

ObjectData::ObjectData(string name, ID3D11Device* device, vector<VertexUV> vertexes, std::vector<unsigned int> indices, D3D_PRIMITIVE_TOPOLOGY primitiveTopology)
{
	this->name = name;
	this->primitiveTopology = primitiveTopology;

	indexCount = (unsigned int)indices.size();
	vertexCount = (unsigned int)vertexes.size();

	HRESULT hr;
	if (indexCount != 0)
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
		hr = device->CreateBuffer(&indexBufferDesc, &iinitData, &indexBuffer);
		if (FAILED(hr))
			cout << "Failed to create index buffer!" << endl;
	}
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = (2 * sizeof(DirectX::XMFLOAT3) + sizeof(DirectX::XMFLOAT2)) * (UINT)vertexes.size();

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertexes.data();
	hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	if (FAILED(hr))
		cout << "Failed to create vertex buffer!" << endl;
}

ObjectData::ObjectData(string name, string fileName, ID3D11Device * device)
{
	this->name = name;
	this->primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	vector<Vector3>crateVertexList(0);
	vector<Vector2>crateUvList(0);
	vector<Vector3>crateNormalList(0);
	vector<Vector3>crateIndexList(0); // vertex, texture, normal // integers here
	string mtlFilename = "";

	vector<VertexUV>crateVertexes(0);
	
	ifstream objFile;
	objFile.open(fileName);
	if (objFile.is_open())
	{
		char check;
		string number = "";
		while (!objFile.eof())
		{
			check = objFile.get();
			if (check == 'v')
			{
				Vector3 xyz;
				Vector2 UV;
				check = objFile.get();
				if (check == ' ') //Vertex extraction
				{
					for (int i = 0; i < 3; i++)
					{
						number = ExtractNumber(check, objFile);
						if (i == 0)
							stringstream(number) >> xyz.x;
						if (i == 1)
							stringstream(number) >> xyz.y;
						if (i == 2)
							stringstream(number) >> xyz.z;
					}
					crateVertexList.push_back(xyz);
				}
				else if (check == 't') // Texture coordinate extraction
				{
					for (int i = 0; i < 2; i++)
					{
						number = ExtractNumber(check, objFile);
						if (i == 0)
							stringstream(number) >> UV.x;
						if (i == 1)
						{
							stringstream(number) >> UV.y;
							UV.y = 1.f - UV.y; // blender uses opengl as it's rendering API. (0, 0) in opengl is bottom left while (0, 0) is top left in direct X
						}
					}
					crateUvList.push_back(UV);
				}
				else if (check == 'n') // Vertex normal extraction
				{
					for (int i = 0; i < 3; i++)
					{
						number = ExtractNumber(check, objFile);
						if (i == 0)
							stringstream(number) >> xyz.x;
						if (i == 1)
							stringstream(number) >> xyz.y;
						if (i == 2)
							stringstream(number) >> xyz.z;
					}
					crateNormalList.push_back(xyz);
				}
			}
			else if (check == 'f') //indexes extration
			{
				number = "";
				check = objFile.get();
				for (int i = 0; i < 3; i++)
				{
					Vector3 indexes;
					for (int j = 0; j < 3; j++)
					{
						check = objFile.get();
						do
						{
							number = number + check;
							check = objFile.get();
						} while (check != '/' && check != ' ' && check != '\n');
						if (j == 0)
							stringstream(number) >> indexes.x;
						if (j == 1)
							stringstream(number) >> indexes.y;
						if (j == 2)
							stringstream(number) >> indexes.z;
						number = "";
						if (check == ' ' || check == '\n')
						{
							crateIndexList.push_back(indexes);
						}
					}
				}
			}
			else if (check == '#' || check == 'g' || check == 'u')
			{
				string line;
				getline(objFile, line);
			}
			else if (check == 'm')
			{
				mtlFilename = ExtractNumber(check, objFile); //This removes the remaining characters in the line
															 //check = objFile.get(); //removes the space
				mtlFilename = ExtractNumber(check, objFile); //Extracts the filename
			}
		}
		objFile.close();

		for (unsigned int i = 0; i < crateIndexList.size(); i++)
		{
			VertexUV vertex;
			// Need to add a -1 for each index because counting in obj format starts at 1
			vertex.position = crateVertexList[(int)(crateIndexList[i].x) - 1];
			vertex.UV = crateUvList[(int)(crateIndexList[i].y) - 1];
			vertex.normal = crateNormalList[(int)(crateIndexList[i].z) - 1];

			crateVertexes.push_back(vertex);
		}
		indexCount = 0;
		vertexCount = (unsigned int)crateVertexes.size();
	}
	else
		cout << "Can't open obj file." << endl;

	HRESULT hr;

	if (indexCount != 0)
	{
		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned int) * (UINT)crateIndexList.size();
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = crateIndexList.data();
		hr = device->CreateBuffer(&indexBufferDesc, &iinitData, &indexBuffer);
		if (FAILED(hr))
			cout << "Failed to create index buffer!" << endl;
	}

	//Crate vertex buffer creation
	D3D11_BUFFER_DESC crateBufferDesc;
	memset(&crateBufferDesc, 0, sizeof(crateBufferDesc));
	crateBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	crateBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	crateBufferDesc.ByteWidth = sizeof(VertexUV) * crateVertexes.size();

	D3D11_SUBRESOURCE_DATA crateData;
	crateData.pSysMem = crateVertexes.data();
	hr = device->CreateBuffer(&crateBufferDesc, &crateData, &vertexBuffer);
	if FAILED(hr)
		cout << "Failed to create vertexshader constant buffer containing " << name << " vertices." << endl;

	//if mtlfile exists
	if (mtlFilename != "")
	{
		string mapKD = "";
		string placeholder;
		ifstream mtlFile;
		mtlFile.open("Resources/" + mtlFilename);
		if (mtlFile.is_open())
		{
			char mtlCheck;
			string mltNumber = "";
			while (!mtlFile.eof())
			{
				mtlCheck = mtlFile.get();
				if (mtlCheck == 'N')
				{
					mtlCheck = mtlFile.get();
					if (mtlCheck == 's')
					{
						mtlCheck = mtlFile.get(); //remove space before number
						placeholder = ExtractNumber(mtlCheck, mtlFile);
						stringstream(placeholder) >> lightData.specularPower;
					}
				}
				else if (mtlCheck == 'K')
				{
					mtlCheck = mtlFile.get();
					if (mtlCheck == 'a')
					{
						mtlCheck = mtlFile.get(); //remove space before number
						placeholder = ExtractNumber(mtlCheck, mtlFile);
						stringstream(placeholder) >> lightData.ambientColor.x;
						placeholder = ExtractNumber(mtlCheck, mtlFile);
						stringstream(placeholder) >> lightData.ambientColor.y;
						placeholder = ExtractNumber(mtlCheck, mtlFile);
						stringstream(placeholder) >> lightData.ambientColor.z;
					}
					else if (mtlCheck == 'd')
					{
						mtlCheck = mtlFile.get(); //remove space before number
						placeholder = ExtractNumber(mtlCheck, mtlFile);
						stringstream(placeholder) >> lightData.diffuseColor.x;
						placeholder = ExtractNumber(mtlCheck, mtlFile);
						stringstream(placeholder) >> lightData.diffuseColor.y;
						placeholder = ExtractNumber(mtlCheck, mtlFile);
						stringstream(placeholder) >> lightData.diffuseColor.z;
					}
					else if (mtlCheck == 's')
					{
						mtlCheck = mtlFile.get(); //remove space before number
						placeholder = ExtractNumber(mtlCheck, mtlFile);
						stringstream(placeholder) >> lightData.specularColor.x;
						placeholder = ExtractNumber(mtlCheck, mtlFile);
						stringstream(placeholder) >> lightData.specularColor.y;
						placeholder = ExtractNumber(mtlCheck, mtlFile);
						stringstream(placeholder) >> lightData.specularColor.z;
					}
				}
				else if (mtlCheck == '#' || mtlCheck == 'n' || mtlCheck == 'i' || mtlCheck == 'd')
				{
					string line;
					getline(mtlFile, line);
				}
				else if (mtlCheck == 'm')
				{
					mapKD = ExtractNumber(mtlCheck, mtlFile); //This removes the remaining characters in the line
					mapKD = ExtractNumber(mtlCheck, mtlFile); //Extracts the filename
					mapKD = "Resources/" + mapKD;
				}
			}
		}
		mtlFile.close();
		if (mapKD != "")
		{
			wstring wmapKD;
			for (unsigned int i = 0; i < mapKD.length(); ++i)
				wmapKD += wchar_t(mapKD[i]);

			hr = DirectX::CreateWICTextureFromFile(device, wmapKD.c_str(), &texture, &textureView);
		}
		cout << "Loaded obj file: " << fileName << endl;
	}
}

string ObjectData::getName() const
{
	return name;
}

ID3D11Buffer ** const ObjectData::getVertexBuffer()
{
	return &vertexBuffer;
}

ID3D11Buffer ** const ObjectData::getIndexBuffer()
{
	return &indexBuffer;
}

ID3D11ShaderResourceView ** const ObjectData::getTextureView()
{
	return &textureView;
}

unsigned int ObjectData::getVertexCount() const
{
	return vertexCount;
}

unsigned int ObjectData::getIndexCount() const
{
	return indexCount;
}

D3D_PRIMITIVE_TOPOLOGY ObjectData::getPrimitiveTopology() const
{
	return primitiveTopology;
}

void ObjectData::addOject(Object * object)
{
	objects.push_back(object);
}

void ObjectData::Draw(ID3D11DeviceContext * deviceContext, Matrix VPMatrix, ID3D11Buffer* cbuffer)
{
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		updateBuffer(deviceContext, VPMatrix, objects[i]->getWorldMatrix(), cbuffer);
		if (indexCount == 0)
		{
			deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
			deviceContext->Draw(vertexCount, 0);
		}
		else
		{
			deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			deviceContext->DrawIndexed(indexCount, 0, 0);
		}
	}
}

void ObjectData::updateBuffer(ID3D11DeviceContext * deviceContext, Matrix VPMatrix, Matrix worldMatrix, ID3D11Buffer* cbuffer)
{
	HRESULT hr;
	//	Disable GPU access to the constant buffer data.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = deviceContext->Map(cbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if FAILED(hr)
		cout << "Failed to disable gpu access to constant buffer." << endl;
	//	Update the constant buffer here.
	WVP_BUFFER* dataptr = (WVP_BUFFER*)mappedResource.pData;
	dataptr->WVPMatrix = XMMatrixTranspose(worldMatrix * VPMatrix);
	dataptr->WorldMatrix = XMMatrixTranspose(worldMatrix);
	//	Reenable GPU access to the constant buffer data.
	deviceContext->Unmap(cbuffer, 0);
}

ObjectData::~ObjectData()
{
	vertexBuffer->Release();
	if (indexCount != 0)
		indexBuffer->Release();

	if (texture != nullptr)
		texture->Release();
	
	if (textureView != nullptr)
		textureView->Release();
}

string ObjectData::ExtractNumber(char & check, ifstream & objFile)
{
	string number = "";
	check = objFile.get();
	do
	{
		number = number + check;
		check = objFile.get();
	} while (check != ' ' && check != '\n');
	if (check == '\n')
	{
		return number;
		number = "";
	}
	else
	{
		return number;
		number = "";
	}
}
