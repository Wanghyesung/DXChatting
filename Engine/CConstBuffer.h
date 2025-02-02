#pragma once
#include "CEntity.h"

class CConstBuffer : public CEntity
{
public:
	CConstBuffer(UINT _iRegisterNum);
	virtual ~CConstBuffer();


	void Create(UINT _iElementSize, UINT _iElementCount);
	void SetData(void* _src, UINT _iSize);
	void UpdateData();
	void UpdateData_CS();

private:
	ComPtr<ID3D11Buffer> m_ConstBuffer;
	D3D11_BUFFER_DESC m_desc;
	const UINT m_iRegisterNum;

	UINT m_iElementSize;
	UINT m_iElementCount;

};

