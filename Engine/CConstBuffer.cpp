#include "pch.h"
#include "CConstBuffer.h"
#include "CDevice.h"

CConstBuffer::CConstBuffer(UINT _iRegisterNum):
	m_iRegisterNum(_iRegisterNum),
	m_desc{},
	m_ConstBuffer(nullptr),
	m_iElementCount(0),
	m_iElementSize(0)
{
}

CConstBuffer::~CConstBuffer()
{

}

void CConstBuffer::Create(UINT _iElementSize, UINT _iElementCount)
{
	m_iElementSize = _iElementSize;
	m_iElementCount = _iElementCount;
	UINT iBufferSize = m_iElementSize * m_iElementCount;

	assert(iBufferSize % 16 == 0); //16바이트 정렬

	m_desc.ByteWidth = iBufferSize;
	m_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_desc.Usage =  D3D11_USAGE_DYNAMIC;			//CPU가 주기적으로 데이터를 업데이트하고, GPU는 읽기 전용으로 사
	m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPU가 주기적으로 데이터를 업데이트하고, GPU는 읽기 전용으로 사

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_desc, nullptr, m_ConstBuffer.ReleaseAndGetAddressOf())))
	{
		assert(nullptr);
	}

}

void CConstBuffer::SetData(void* _src, UINT _iSize)
{
	UINT iSize = _iSize;
	if (iSize == 0)
	{
		iSize = m_iElementSize * m_iElementCount;
	}

	assert(_iSize % 16 == 0);

	//CPU에서 GPU데이터 전달

	//D3D11_MAP_WRITE_DISCARD: 기존 데이터를 버리고 새로운 데이터로 덮어씁니다.
	D3D11_MAPPED_SUBRESOURCE sub = {};
	//sub.pData가 실제 GPU 메모리를 가리킨다
	if (! FAILED(CDevice::GetInst()->GetContext()->Map(m_ConstBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &sub)))
	{
		memcpy(sub.pData, _src, iSize);
		CDevice::GetInst()->GetContext()->Unmap(m_ConstBuffer.Get(), 0); //unmap을 사용해야 GPU가 다시 접근가능
	}
}

void CConstBuffer::UpdateData()
{
	CDevice::GetInst()->GetContext()->VSSetConstantBuffers(m_iRegisterNum, 1, m_ConstBuffer.GetAddressOf());
	CDevice::GetInst()->GetContext()->DSSetConstantBuffers(m_iRegisterNum, 1, m_ConstBuffer.GetAddressOf());
	CDevice::GetInst()->GetContext()->HSSetConstantBuffers(m_iRegisterNum, 1, m_ConstBuffer.GetAddressOf());
	CDevice::GetInst()->GetContext()->GSSetConstantBuffers(m_iRegisterNum, 1, m_ConstBuffer.GetAddressOf());
	CDevice::GetInst()->GetContext()->PSSetConstantBuffers(m_iRegisterNum, 1, m_ConstBuffer.GetAddressOf());

}

void CConstBuffer::UpdateData_CS()
{
	CDevice::GetInst()->GetContext()->CSSetConstantBuffers(m_iRegisterNum, 1, m_ConstBuffer.GetAddressOf());
}
