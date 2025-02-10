#pragma once
#include "CEntity.h"
class CStructuredBuffer : public CEntity
{
public:
	CStructuredBuffer();
	~CStructuredBuffer();

public:
	void SetData(void* _src, UINT _iSize);
	void GetData(void* _dst, UINT _iSize);

	void Create(UINT _iElementSize, UINT _iElementCount, BUFFER_TYPE _eType, bool _bSysAccess, void* _sysMem);
	void Clear();

	void UpdateData_CS(UINT _iRegisterNum, bool _bUAV);
	void UpdateData(UINT _iRegisterNum, SHADER_FLAG _eFlag);


private:
	ComPtr<ID3D11Buffer> m_SB;
	ComPtr<ID3D11ShaderResourceView> m_SB_SRV;
	ComPtr<ID3D11UnorderedAccessView> m_SB_UAV;

	ComPtr<ID3D11Buffer> m_SB_CPU_Writer;
	ComPtr<ID3D11Buffer> m_SB_CPU_Reader;

	D3D11_BUFFER_DESC m_bufferDesc;

	UINT m_iRegisterNum;
	BUFFER_TYPE  m_eBufferType;
	bool m_bSysAccess;

	UINT m_iElementSize;
	UINT m_iElementCount;
};

