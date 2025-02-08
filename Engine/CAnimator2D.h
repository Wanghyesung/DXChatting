#pragma once
#include "CComponent.h"
#include "CAnimation2D.h"

class CTexture;;

class CAnimator2D : public CComponent
{
public:
	CAnimator2D();
	~CAnimator2D();

private:
	map<string, CAnimation2D*> m_mapAnimation;
};

