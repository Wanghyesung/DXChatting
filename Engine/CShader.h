#pragma once
#include "CResource.h"

class CShader : public CResource
{
public:
	CShader();
	virtual ~CShader() {};

public:
	virtual void UpdateData() = 0;

protected:
	ComPtr<ID3DBlob> m_errorBlob;
};

