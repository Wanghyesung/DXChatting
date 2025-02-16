#pragma once
#include "CResource.h"

class CTexture;
class CGraphicsShader;
class CMaterial : public CResource
{
public:
	CMaterial();
	CMaterial(const CMaterial& _otehr);
	virtual ~CMaterial();

	
public:
	virtual void UpdateData();
	void SetShader(shared_ptr<CGraphicsShader> _pShader) { m_pShader = _pShader; }
	void SetTex(TEX_PARAM _iIdx, shared_ptr<CTexture> _pTex){ m_arrTex[(UINT)_iIdx] = _pTex;}
	void SetMtrlParm(MTRL_PARAM _eParm, void* _src);

	RENDER_TYPE GetRenderType() { return m_eRenderType; }

public:
	virtual CMaterial* Clone() override{ return new CMaterial(*this); }

protected:
	virtual int Load(const wstring& _strPath) { return S_OK; }
	virtual int Save(const wstring& _strPath) { return S_OK; }

private:
	shared_ptr<CGraphicsShader> m_pShader;
	shared_ptr<CTexture> m_arrTex[(UINT)TEX_PARAM::TEX_END];
	tMtrlConst m_mtrlConst;
	RENDER_TYPE m_eRenderType;
};

