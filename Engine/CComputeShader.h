#pragma once
#include "CShader.h"

class CComputeShader : public CShader
{
public:
    CComputeShader();
    virtual ~CComputeShader();

	virtual void UpdateData() = 0;
    virtual void Clear() = 0;

    void CreateComputeShader(const wstring& _strFileName, const string& _strFuncName);
    void Excute();
   
private:
    
    ComPtr<ID3D11ComputeShader> m_pComputeShader;
    ComPtr<ID3DBlob> m_pBlob;

protected:
    tMtrlConst m_tMtrl;

    // 그룹 개수
    UINT                            m_iGroupX;
    UINT                            m_iGroupY;
    UINT                            m_iGroupZ;

    // 그룹 1개당 스레드 개수
    UINT                            m_iGroupThreadX;
    UINT                            m_iGroupThreadY;
    UINT                            m_iGroupThreadZ;

	
};

