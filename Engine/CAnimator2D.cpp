#include "pch.h"
#include "CAnimator2D.h"
#include "CMeshRender.h"
#include "CTexture.h"

CAnimator2D::CAnimator2D():
	CComponent(COMPONENT_TYPE::ANIMATOR),
	m_mapAnimation{}
{
}

CAnimator2D::~CAnimator2D()
{

}

void CAnimator2D::begin()
{

}

void CAnimator2D::final_tick()
{
	if (m_pCurAnim == nullptr)
		return;
	if (m_pCurAnim->m_bCompleted)
	{
		m_pCurAnim->reset();
	}
	m_pCurAnim->finaltick();
}

void CAnimator2D::UpdateData()
{
	shared_ptr<CMaterial> pMtrl = GetComponent<CMeshRender>(COMPONENT_TYPE::MESH_RENDER)->GetMaterial();
	int iAnimUse = 1;

	tAnim2DFrame tFrame = m_pCurAnim->GetCurAnimFrame();
	Vector2 vBackSize = m_pCurAnim->GetBackSize();

	pMtrl->SetMtrlParm(INT_0, &iAnimUse);
	pMtrl->SetMtrlParm(VEC2_0, &tFrame.LeftTopUV);
	pMtrl->SetMtrlParm(VEC2_1, &tFrame.SliceUV);
	pMtrl->SetMtrlParm(VEC2_2, &tFrame.Offset);
	pMtrl->SetMtrlParm(VEC2_3, &vBackSize);

	pMtrl->SetTex(TEX_0, m_pCurAnim->m_pAtlasTex);
}

void CAnimator2D::Create(const wstring& _strName, shared_ptr<CTexture> _pAtlasTex, Vector2 _vLeftTop, Vector2 _vSliceSize, Vector2 _vBackSize, UINT _iFrameSize, float _fFPS)
{
	if (FindAnim2D(_strName) != nullptr)
		return;

	CAnimation2D* pAnim = new CAnimation2D();
	pAnim->Create(_strName, _pAtlasTex, _vLeftTop, _vSliceSize, _vBackSize, _iFrameSize,_fFPS);
	pAnim->m_pOwner = this;

	m_mapAnimation.insert(make_pair(_strName, pAnim));

}

CAnimation2D* CAnimator2D::FindAnim2D(const wstring& _strName)
{
	map<wstring, CAnimation2D*>::iterator iter;
	if ((iter = m_mapAnimation.find(_strName)) != m_mapAnimation.end())
		return iter->second;

	return nullptr;
}
