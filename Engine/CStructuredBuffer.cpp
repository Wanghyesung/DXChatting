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
	//구조화 버퍼의 전체 크기
	UINT iSize = _iSize;
	if (iSize == 0)
		iSize = m_iElementSize * m_iElementCount;

	//GPU 리소스는 기본적으로 CPU가 접근할 수 없으므로, CPU 접근이 가능한 버퍼 m_SB_CPU_Writer 사용
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	//D3D11_MAP_WRITE_DISCARD 기존 데이터를 무시하고 새로운 데이터를 덮어씀
	CDevice::GetInst()->GetContext()->Map(m_SB_CPU_Writer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(&tSub.pData, _src, iSize);
	CDevice::GetInst()->GetContext()->Unmap(m_SB_CPU_Writer.Get(),0);


	CDevice::GetInst()->GetContext()->CopyResource(m_SB.Get(), m_SB_CPU_Writer.Get());
}

void CStructuredBuffer::GetData(void* _dst, UINT _iSize)
{
	//GPU에서 계산된 데이터(m_SB)를 CPU에서 접근할 수 있는 버퍼(m_SB_CPU_Reader)로 복사
	CDevice::GetInst()->GetContext()->CopyResource(m_SB_CPU_Reader.Get(), m_SB.Get());

	UINT iSize = _iSize;
	if (iSize == 0)
		iSize = m_iElementSize * m_iElementCount;

	// GPU 데이터를 CPU가 읽을 수 있도록 D3D11_MAP_READ로 매핑
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	// CPU에서 사용할 _dst 버퍼에 데이터 복사
	CDevice::GetInst()->GetContext()->Map(m_SB_CPU_Reader.Get(), 0, D3D11_MAP::D3D11_MAP_READ, 0, &tSub);
	//memcpy(_dst, tSub.pData, iSize); → tSub.pData는 매핑된 GPU 데이터의 주소
	memcpy(_dst, tSub.pData, iSize);
	//Unmap()을 호출하여 GPU 버퍼를 다시 해제
	CDevice::GetInst()->GetContext()->Unmap(m_SB_CPU_Reader.Get(), 0);
}

void CStructuredBuffer::Create(UINT _iElementSize, UINT _iElementCount, BUFFER_TYPE _eType, bool _bSysAccess, void* _sysMem)
{
	m_bSysAccess = _bSysAccess;					// 시스템 메모리 접근 여부
	m_eBufferType = _eType;						// 버퍼 유형 (READ / WRITE)

	m_iElementSize = _iElementSize;				// 요소 하나의 크기 (바이트 단위)
	m_iElementCount = _iElementCount;			// 전체 요소 개수

	UINT iBufferSize = m_iElementSize * m_iElementCount;

	m_bufferDesc.ByteWidth = iBufferSize;				 //전체 버퍼 크기
	m_bufferDesc.StructureByteStride = m_iElementSize;   //구조화 버퍼의 요소 간 간격
	
	if (m_eBufferType == BUFFER_TYPE::READ)
		m_bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	else
		m_bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS; //셰이더에서 읽고 쓰기 가능

	m_bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;	// 구조화 버퍼로 설정
	m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//CPU 접근 불가, GPU에서만 읽고 씀
	m_bufferDesc.CPUAccessFlags = 0;								//CPU가 접근하지 않음.

	if (_sysMem == nullptr)
	{
		if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_bufferDesc, nullptr, m_SB.GetAddressOf())))
			assert(nullptr);
	}
	else
	{
		//초기 데이터가 있으면 해당 데이터로 버퍼 생성
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
		//UAV는 셰이더에서 읽고 쓰기가 가능
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

		// 데이터 읽기용 버퍼 (GPU → CPU)
		m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_bufferDesc, nullptr, m_SB_CPU_Reader.GetAddressOf())))
			assert(nullptr);

		// 데이터 쓰기용 버퍼 (CPU → GPU)
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
