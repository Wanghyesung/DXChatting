#include "pch.h"
#include "CMesh.h"
#include "CDevice.h"

CMesh::CMesh():
    m_indexBuffer{},
    m_vertexBuffer{},
    m_vertexBufferDesc{},
    m_indexBufferDesc{}
{
}

CMesh::~CMesh()
{

}

int CMesh::Create(void* _pVertex, UINT _iVertexCount, void* _pIndex, UINT _iIndexCount)
{
    m_iVertexCount = _iVertexCount;

    m_vertexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
    m_vertexBufferDesc.ByteWidth = sizeof(tVertex) * m_iVertexCount;
    m_vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT; //CPU�� �������� �ʰ� GPU������ ���
   
    /*
    D3D11_USAGE_DYNAMIC �� D3D11_CPU_ACCESS_WRITE �ʿ�.
    D3D11_USAGE_STAGING �� D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE
    */

    m_vertexBufferDesc.CPUAccessFlags = 0;//0�̸� GPU������ ���
    m_vertexBufferDesc.MiscFlags = 0;
    m_vertexBufferDesc.StructureByteStride = 0; //����ȭ ���� ���X

    //�ʱ� �����͸� GPU�� ����
    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = _pVertex;

    if(FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_vertexBufferDesc, &data, m_vertexBuffer.GetAddressOf())))
    {
        assert(nullptr);
    }

   
    m_iIndexCount = _iIndexCount;

    m_indexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
    m_indexBufferDesc.ByteWidth = sizeof(UINT) * _iIndexCount;
    m_indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    m_indexBufferDesc.CPUAccessFlags = 0;
    m_indexBufferDesc.MiscFlags = 0;
    m_indexBufferDesc.StructureByteStride = 0;

    data = {};
    data.pSysMem = _pIndex;

    if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_indexBufferDesc, &data, m_indexBuffer.GetAddressOf())))
    {
        assert(nullptr);
    }

    m_pVertex = new tVertex[m_iVertexCount];
    memcpy(m_pVertex, _pVertex, sizeof(tVertex) * m_iVertexCount);

    m_pIndex = new UINT[m_iIndexCount];
    memcpy(m_pIndex, _pIndex, sizeof(UINT) * m_iIndexCount);

    return S_OK;
}

void CMesh::UpdateData()
{
    //������ ����������(�Է� �����, IA)�� ���ε�
    UINT iStride = sizeof(tVertex);
    UINT iOffset = 0;

    CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &iStride, &iOffset);
    CDevice::GetInst()->GetContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::Render()
{
    //������ ���� ���۸� ���ε�
    UpdateData();

    CDevice::GetInst()->GetContext()->DrawIndexed(m_iIndexCount,0,0);
}

int CMesh::Load(const wstring& _strPath)
{
    return 0;
}

int CMesh::Save(const wstring& _strPath)
{
    return 0;
}
