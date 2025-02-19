#include "pch.h"
#include "CSpeechBubble.h"
#include "CMeshRender.h"
#include "CTransform.h"
#include "CResMgr.h"

CSpeechBubble::CSpeechBubble()
{

}

CSpeechBubble::~CSpeechBubble()
{

}

void CSpeechBubble::init(bool _bClientBubble)
{
	//내가 보낸 채팅인지 상대방인지 구분

	SetComponent(new CTransform());

	shared_ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"BubbleMaterial1", RESOURCE_TYPE::MATERIAL);
	shared_ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	CMeshRender* pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
	pMeshRender->SetMesh(pMesh);
	SetComponent(pMeshRender);
}

void CSpeechBubble::tick()
{
	CSpeechObject::tick();
}

void CSpeechBubble::MouseOn()
{
	CSpeechObject::MouseOn();

}

void CSpeechBubble::MouseLbtnDown()
{
	CSpeechObject::MouseLbtnDown();

}

void CSpeechBubble::MouseLbtnUp()
{
	CSpeechObject::MouseLbtnUp();

}

void CSpeechBubble::MouseLbtnClicked()
{
	CSpeechObject::MouseLbtnClicked();

}
