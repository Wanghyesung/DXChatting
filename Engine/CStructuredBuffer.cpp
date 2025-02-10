#include "pch.h"
#include "CStructuredBuffer.h"
#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
{

}

CStructuredBuffer::~CStructuredBuffer()
{

}

void CStructuredBuffer::SetData(void* _src, UINT _iSize)
{
	//����ȭ ������ ��ü ũ��
	UINT iSize = _iSize;
	if (iSize == 0)
		iSize = m_iElementSize * m_iElementCount;

	//GPU ���ҽ��� �⺻������ CPU�� ������ �� �����Ƿ�, CPU ������ ������ ���� m_SB_CPU_Writer ���
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	//D3D11_MAP_WRITE_DISCARD ���� �����͸� �����ϰ� ���ο� �����͸� ���
	CDevice::GetInst()->GetContext()->Map(m_SB_CPU_Writer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(&tSub.pData, _src, iSize);
	CDevice::GetInst()->GetContext()->Unmap(m_SB_CPU_Writer.Get(),0);


	CDevice::GetInst()->GetContext()->CopyResource(m_SB.Get(), m_SB_CPU_Writer.Get());
}

void CStructuredBuffer::GetData(void* _dst, UINT _iSize)
{
	//GPU���� ���� ������(m_SB)�� CPU���� ������ �� �ִ� ����(m_SB_CPU_Reader)�� ����
	CDevice::GetInst()->GetContext()->CopyResource(m_SB_CPU_Reader.Get(), m_SB.Get());

	UINT iSize = _iSize;
	if (iSize == 0)
		iSize = m_iElementSize * m_iElementCount;

	// GPU �����͸� CPU�� ���� �� �ֵ��� D3D11_MAP_READ�� ����
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	// CPU���� ����� _dst ���ۿ� ������ ����
	CDevice::GetInst()->GetContext()->Map(m_SB_CPU_Reader.Get(), 0, D3D11_MAP::D3D11_MAP_READ, 0, &tSub);
	//memcpy(_dst, tSub.pData, iSize); �� tSub.pData�� ���ε� GPU �������� �ּ�
	memcpy(_dst, tSub.pData, iSize);
	//Unmap()�� ȣ���Ͽ� GPU ���۸� �ٽ� ����
	CDevice::GetInst()->GetContext()->Unmap(m_SB_CPU_Reader.Get(), 0);
}

void CStructuredBuffer::Create(UINT _iElementSize, UINT _iElementCount, BUFFER_TYPE _eType, bool _bSysAccess, void* _sysMem)
{
	m_bSysAccess = _bSysAccess;					// �ý��� �޸� ���� ����
	m_eBufferType = _eType;						// ���� ���� (READ / WRITE)

	m_iElementSize = _iElementSize;				// ��� �ϳ��� ũ�� (����Ʈ ����)
	m_iElementCount = _iElementCount;			// ��ü ��� ����

	UINT iBufferSize = m_iElementSize * m_iElementCount;

	m_bufferDesc.ByteWidth = iBufferSize;				 //��ü ���� ũ��
	m_bufferDesc.StructureByteStride = m_iElementSize;   //����ȭ ������ ��� �� ����
	
	if (m_eBufferType == BUFFER_TYPE::READ)
		m_bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	else
		m_bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS; //���̴����� �а� ���� ����

	m_bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;	// ����ȭ ���۷� ����
	m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//CPU ���� �Ұ�, GPU������ �а� ��
	m_bufferDesc.CPUAccessFlags = 0;								//CPU�� �������� ����.

	if (_sysMem == nullptr)
	{
		if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_bufferDesc, nullptr, m_SB.GetAddressOf())))
			assert(nullptr);
	}
	else
	{
		//�ʱ� �����Ͱ� ������ �ش� �����ͷ� ���� ����
		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = _sysMem;
		if(FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_bufferDesc, &subData, m_SB.GetAddressOf())))
			assert(nullptr);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	//SRV
	SRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.NumElements = m_iElementCount;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_SB.Get(), &SRVDesc, m_SB_SRV.GetAddressOf())))
		assert(nullptr);

	if (m_eBufferType == BUFFER_TYPE::WRITE)
	{
		//UAV�� ���̴����� �а� ���Ⱑ ����
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		//UAV
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER; 
		UAVDesc.Buffer.NumElements = m_iElementCount;

		if (FAILED(CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_SB.Get(), &UAVDesc, m_SB_UAV.GetAddressOf())))
		{
			assert(nullptr);
		}
	}


	//
	if (m_bSysAccess)
	{
		m_bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;

		// ������ �б�� ���� (GPU �� CPU)
		m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_bufferDesc, nullptr, m_SB_CPU_Reader.GetAddressOf())))
			assert(nullptr);

		// ������ ����� ���� (CPU �� GPU)
		m_bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_bufferDesc, nullptr, m_SB_CPU_Writer.GetAddressOf())))
			assert(nullptr);
	}

}

void CStructuredBuffer::UpdateData_CS(UINT _iRegisterNum, bool _bUAV)
{
	m_iRegisterNum = _iRegisterNum;

	const UINT i= -1;
	if (_bUAV)
		CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(m_iRegisterNum, 1, m_SB_UAV.GetAddressOf(),&i);
	else
		CDevice::GetInst()->GetContext()->CSSetShaderResources(m_iRegisterNum, 1, m_SB_SRV.GetAddressOf());
}

void CStructuredBuffer::UpdateData(UINT _iRegisterNum, SHADER_FLAG _eFlag)
{
	m_iRegisterNum = _iRegisterNum;

	if (SHADER_FLAG::VS & _eFlag)
		CDevice::GetInst()->GetContext()->VSSetShaderResources(m_iRegisterNum, 1, m_SB_SRV.GetAddressOf());
	if (SHADER_FLAG::DS & _eFlag)			  
		CDevice::GetInst()->GetContext()->DSSetShaderResources(m_iRegisterNum, 1, m_SB_SRV.GetAddressOf());
	if (SHADER_FLAG::HS & _eFlag)			  
		CDevice::GetInst()->GetContext()->HSSetShaderResources(m_iRegisterNum, 1, m_SB_SRV.GetAddressOf());
	if (SHADER_FLAG::GS & _eFlag)			 
		CDevice::GetInst()->GetContext()->GSSetShaderResources(m_iRegisterNum, 1, m_SB_SRV.GetAddressOf());
	if (SHADER_FLAG::PS & _eFlag)
		CDevice::GetInst()->GetContext()->PSSetShaderResources(m_iRegisterNum, 1, m_SB_SRV.GetAddressOf());
	
}

void CStructuredBuffer::Clear()
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	CDevice::GetInst()->GetContext()->VSSetShaderResources(m_iRegisterNum, 1,&pSRV);
	CDevice::GetInst()->GetContext()->DSSetShaderResources(m_iRegisterNum, 1,&pSRV);
	CDevice::GetInst()->GetContext()->HSSetShaderResources(m_iRegisterNum, 1,&pSRV);
	CDevice::GetInst()->GetContext()->GSSetShaderResources(m_iRegisterNum, 1,&pSRV);
	CDevice::GetInst()->GetContext()->PSSetShaderResources(m_iRegisterNum, 1,&pSRV);
	CDevice::GetInst()->GetContext()->CSSetShaderResources(m_iRegisterNum, 1,&pSRV);

}
