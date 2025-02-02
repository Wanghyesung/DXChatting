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

	assert(iBufferSize % 16 == 0); //16����Ʈ ����

	m_desc.ByteWidth = iBufferSize;
	m_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_desc.Usage =  D3D11_USAGE_DYNAMIC;			//CPU�� �ֱ������� �����͸� ������Ʈ�ϰ�, GPU�� �б� �������� ��
	m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPU�� �ֱ������� �����͸� ������Ʈ�ϰ�, GPU�� �б� �������� ��

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

	//CPU���� GPU������ ����

	//D3D11_MAP_WRITE_DISCARD: ���� �����͸� ������ ���ο� �����ͷ� ����ϴ�.
	D3D11_MAPPED_SUBRESOURCE sub = {};
	//sub.pData�� ���� GPU �޸𸮸� ����Ų��
	if (! FAILED(CDevice::GetInst()->GetContext()->Map(m_ConstBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &sub)))
	{
		memcpy(sub.pData, _src, iSize);
		CDevice::GetInst()->GetContext()->Unmap(m_ConstBuffer.Get(), 0); //unmap�� ����ؾ� GPU�� �ٽ� ���ٰ���
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
