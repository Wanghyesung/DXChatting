#include "pch.h"
#include "CAnimation2D.h"
#include"CTexture.h"
#include "CTimeMgr.h"
CAnimation2D::CAnimation2D():
	m_iCurFrame(0),
	m_fCurTime(0.f),
	m_bCompleted(false)
{

}

CAnimation2D::~CAnimation2D()
{

}

void CAnimation2D::Create(const wstring& _strName, shared_ptr<CTexture> _pAtlasTex, Vector2 _vLeftTop, Vector2 _vSliceSize, Vector2 _vBackSize, UINT _iFrameSize, float _fFPS)
{
	m_pAtlasTex = _pAtlasTex;


	//UV ÁÂÇ¥ 0~1
	Vector2 vResolution = Vector2(m_pAtlasTex->GetWidth(), m_pAtlasTex->GetHeight());
	m_vBackSize = _vBackSize / vResolution; 


	for (UINT i = 0; i < _iFrameSize; ++i)
	{
		tAnim2DFrame tFrame = {};
		tFrame.LeftTopUV = Vector2(_vLeftTop.x + (i * _vSliceSize.x), _vSliceSize.y) / vResolution;
		tFrame.fDuration = 1.f / _fFPS;
		tFrame.SliceUV = _vSliceSize / vResolution;

		m_vecFrame.push_back(tFrame);
	}
}

void CAnimation2D::finaltick()
{
	if (m_bCompleted)
		return;

	m_fCurTime += DT;
	if (m_fCurTime >= m_vecFrame[m_iCurFrame].fDuration)
	{
		m_fCurTime -= m_vecFrame[m_iCurFrame].fDuration;
		++m_iCurFrame;

		if (m_iCurFrame >= m_vecFrame.size())
		{
			m_bCompleted = true;
			m_iCurFrame = 0;
		}

	}
}

void CAnimation2D::reset()
{
	m_iCurFrame = 0;
	m_fCurTime = 0;
	m_bCompleted = false;
}
