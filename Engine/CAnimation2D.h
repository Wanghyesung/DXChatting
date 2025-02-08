#pragma once
#include "CEntity.h"

struct tAnim2DFrame
{
	Vec2	LeftTopUV;
	Vec2	SliceUV;
	Vec2	Offset;
	float	fDuration;
};

class CTexture;

class CAnimation2D : public CEntity
{
	friend class CAnimator2D;
public:
	CAnimation2D();
	virtual ~CAnimation2D();

	void Create(const wstring& _strName, shared_ptr<CTexture> _pAtlasTex, Vector2 _vLeftTop, Vector2 _vSliceSize, Vector2 _vBackSize,
				UINT _iFrameSize, float _fFPS);
private:
	void finaltick();

private:
	shared_ptr<CTexture> m_pAtlasTex;
	Vector2 m_vBackSize;
	CAnimator2D* m_pOwner;
	vector< tAnim2DFrame> m_vecFrame;

	UINT m_iCurFrame;
	float m_fCurTime;
};

