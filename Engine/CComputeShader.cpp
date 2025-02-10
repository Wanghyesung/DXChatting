#include "pch.h"
#include "CComputeShader.h"
#include "CDevice.h"
#include "CConstBuffer.h"

//Restype�����ڿ��� ���ϰ�
CComputeShader::CComputeShader():
	m_iGroupThreadX(1),
	m_iGroupThreadY(1),
	m_iGroupThreadZ(1)
{
	
}

CComputeShader::~CComputeShader()
{

}

void CComputeShader::CreateComputeShader(const wstring& _strFileName, const string& _strFuncName)
{
	if (FAILED(D3DCompileFromFile(_strFileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_strFuncName.c_str(), "cs_5_0", 0, 0, m_pBlob.GetAddressOf(), m_errorBlob.GetAddressOf())))
	{
		assert(nullptr);
	}
	

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateComputeShader(
		m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, m_pComputeShader.GetAddressOf())))
	{
		assert(nullptr);
	}
}

void CComputeShader::Excute()
{
	//���������ΰ� �����̱� ������ IA, RS, OM�� �������� �ʾƵ� ��

	UpdateData();

	CConstBuffer* pMtrlConst = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pMtrlConst->SetData(&m_tMtrl, sizeof(tMtrlConst));
	pMtrlConst->UpdateData_CS();

	CDevice::GetInst()->GetContext()->CSSetShader(m_pComputeShader.Get(), nullptr, 0);
	CDevice::GetInst()->GetContext()->Dispatch(m_iGroupX, m_iGroupY, m_iGroupZ);

	//����� ����ȭ���� ����	
}
