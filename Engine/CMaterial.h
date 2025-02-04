#pragma once
#include "CResource.h"

class CTexture;
class CGraphicsShader;
class CMaterial : public CResource
{
public:
	CMaterial();
	virtual ~CMaterial();

	
public:
	virtual void UpdateData();
	void SetShader(shared_ptr<CGraphicsShader> _pShader) { m_pShader = _pShader; }
	void SetTex(shared_ptr<CTexture> _pTex, TEX_PARAM _iIdx){ m_arrTex[(UINT)_iIdx] = _pTex;}
	void SetMtrlParm(MTRL_PARAM _eParm, void* _src);
protected:
	virtual int Load(const wstring& _strPath) = 0;
	virtual int Save(const wstring& _strPath) = 0;

private:
	shared_ptr<CGraphicsShader> m_pShader;
	shared_ptr<CTexture> m_arrTex[(UINT)TEX_PARAM::TEX_END];
	tMtrlConst m_mtrlConst;

};

