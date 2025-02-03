#pragma once
#include "CResource.h"



class CMesh : public CResource
{
public:
	CMesh();
	virtual ~CMesh();

	int Create(void* _pVertex, UINT _iVertexCount, void* _pIndex, UINT _iIndexCount);
	
	void Render();

private:
	virtual void UpdateData();

public:
	virtual int Load(const wstring& _strPath)override;
	virtual int Save(const wstring& _strPath)override;

private:
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;

	D3D11_BUFFER_DESC m_vertexBufferDesc;
	D3D11_BUFFER_DESC m_indexBufferDesc;

	void* m_pVertex;
	void* m_pIndex;

	UINT m_iVertexCount;
	UINT m_iIndexCount;

};

