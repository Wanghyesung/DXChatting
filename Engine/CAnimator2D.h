#pragma once
#include "CComponent.h"
#include "CAnimation2D.h"

class CTexture;;

class CAnimator2D : public CComponent
{
public:
	CAnimator2D();
	~CAnimator2D();

	virtual void begin();
	virtual void tick() {};
	virtual void final_tick();

	void UpdateData();

	void Create(const wstring& _strName, shared_ptr<CTexture> _pAtlasTex, Vector2 _vLeftTop, Vector2 _vSliceSize, Vector2 _vBackSize,
		UINT _iFrameSize, float _fFPS);

	CAnimation2D* FindAnim2D(const wstring& _strname);
	
private:
	map<wstring, CAnimation2D*> m_mapAnimation;
	bool m_bRepet;
	CAnimation2D* m_pCurAnim;
};

