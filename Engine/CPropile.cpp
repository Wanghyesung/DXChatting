#include "pch.h"
#include "CPropile.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CResMgr.h"

CPropile::CPropile() 
{

}

CPropile::~CPropile()
{

}

void CPropile::Init()
{
	CTransform* pTrasnform = new CTransform();
	pTrasnform->SetScale(Vector3{ 50.f,50.f,1.f });//∞Ì¡§
	SetComponent(pTrasnform);

	shared_ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"PropileMaterial", RESOURCE_TYPE::MATERIAL);
	shared_ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	CMeshRender* pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
	pMeshRender->SetMesh(pMesh);
	SetComponent(pMeshRender);

	SetFontSize(15.f);
}

void CPropile::tick()
{
	if (!m_strSpeech.empty())
		CSpeechObject::Speech(m_strSpeech,true,true,Vector2{-10.f,-35.f});

	CUI::tick();
}

void CPropile::MouseOn()
{

}

void CPropile::MouseLbtnDown()
{

}

void CPropile::MouseLbtnUp()
{

}

void CPropile::MouseLbtnClicked()
{

}
