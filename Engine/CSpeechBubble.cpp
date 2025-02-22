#include "pch.h"
#include "CSpeechBubble.h"
#include "CMeshRender.h"
#include "CTransform.h"
#include "CResMgr.h"
#include "CEventMgr.h"

CSpeechBubble::CSpeechBubble():
	m_vOffset(20.f, -10.f)
{

}

CSpeechBubble::~CSpeechBubble()
{

}

void CSpeechBubble::init(bool _bOther)
{
	//내가 보낸 채팅인지 상대방인지 구분

	SetComponent(new CTransform());

	wstring MtrlName = L"BubbleMaterial1";
	if (_bOther)
		MtrlName = L"BubbleMaterial2";

	shared_ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(MtrlName, RESOURCE_TYPE::MATERIAL);
	shared_ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	CMeshRender* pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
	pMeshRender->SetMesh(pMesh);
	SetComponent(pMeshRender);
}

void CSpeechBubble::tick()
{
	CUI::tick();

	int iLen = m_strSpeech.size();
	int iNewLineCnt = iLen / MAX_FONT_COUNT;
	int iYOffset = (iLen - iNewLineCnt) / MAX_FONT_COUNT;
	if (!m_strSpeech.empty())
		Speech(m_strSpeech, false, true, Vector2{ m_vOffset.x, m_vOffset.y * iYOffset });
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

void CSpeechBubble::MouseRelease()
{
	CSpeechObject::MouseRelease();
}
