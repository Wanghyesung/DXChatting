#include "pch.h"
#include "CRoomMgr.h"
#include "CLayer.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CResMgr.h"
#include "CMaterial.h"
#include "CCamera.h"
#include "CUI.h"
#include "CSpeechBar.h"
CRoomMgr::CRoomMgr() :
	m_pCurRoom(nullptr)
{
}

CRoomMgr::~CRoomMgr()
{
	auto iter = m_mapRoom.begin();
	for (iter; iter != m_mapRoom.end(); ++iter)
	{
		delete iter->second;
		iter->second = nullptr;
	}
	m_mapRoom.clear();
}


CRoom* CRoomMgr::FindRoom(const wstring& _strName)
{
	auto iter = m_mapRoom.find(_strName);
	if (iter != m_mapRoom.end())
		return iter->second;

	return nullptr;
}

void CRoomMgr::SetCurRoom(const wstring& _strName)
{
	CRoom* pRoom = FindRoom(_strName);
	if (pRoom == nullptr)
		assert(nullptr);

	m_pCurRoom = pRoom;
}

const vector<CLayer*>& CRoomMgr::GetCurLayers()
{
	if (m_pCurRoom == nullptr)
		assert(nullptr);

	return m_pCurRoom->m_vecLayer;
}

CLayer* CRoomMgr::GetCurLayer(LAYER_TYPE _eLayerType)
{
	if (m_pCurRoom == nullptr)
		assert(nullptr);

	 return m_pCurRoom->m_vecLayer[(UINT)_eLayerType];
}

const vector<CObject*>& CRoomMgr::GetUIs()
{
	return m_pCurRoom->m_vecLayer[(UINT)LAYER_TYPE::UI]->m_vecObject;
}

void CRoomMgr::EraseUI(CObject* _pObject)
{
	vector<CObject*>& vecObj = m_pCurRoom->m_vecLayer[(UINT)LAYER_TYPE::UI]->m_vecObject;
	
	vector<CObject*>::iterator iter = vecObj.begin();
	for (iter; iter != vecObj.end(); ++iter)
	{
		if (*iter == _pObject)
		{
			vecObj.erase(iter);
			return;
		}
	}
}


void CRoomMgr::tick()
{
	m_pCurRoom->tick();

	m_pCurRoom->finaltick();
}

void CRoomMgr::init()
{
	m_pCurRoom = new CRoom();
	m_mapRoom.insert(make_pair(L"lobby", m_pCurRoom));

	CUI* pUI = new CUI();
	pUI->SetName(L"TemObject");
	m_pCurRoom->AddObject(LAYER_TYPE::UI, pUI);
	
	CTransform* pTrasnform = new CTransform();
	pTrasnform->SetPostion(Vector3{0.f,0.f,-0.2f});
	pTrasnform->SetScale(Vector3{ 200.f,200.f,1.f });
	pUI->SetComponent(pTrasnform);


	shared_ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DefaultMaterial", RESOURCE_TYPE::MATERIAL);
	shared_ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	CMeshRender* pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
	pMeshRender->SetMesh(pMesh);

	pMtrl->SetTex(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"TemTex", RESOURCE_TYPE::TEXTURE));
	pUI->SetComponent(pMeshRender);

	/*///////////////////////
			SPEECHBAR
	*////////////////////////
	CSpeechBar* pSpeechBar = new CSpeechBar();
	pUI->SetName(L"SpeechBar");
	m_pCurRoom->AddObject(LAYER_TYPE::UI, pSpeechBar);

	pTrasnform = new CTransform();
	pTrasnform->SetPostion(Vector3{ 200.f,-200.f,-0.2f });
	pTrasnform->SetScale(Vector3{ 200.f,200.f,1.f });
	pSpeechBar->SetComponent(pTrasnform);

	pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DefaultMaterial", RESOURCE_TYPE::MATERIAL);
	pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
	shared_ptr<CMaterial> pDynamicMtrl = pMeshRender->GetDynamicMaterial();
	pMeshRender->SetMaterial(pDynamicMtrl);

	pMeshRender->SetMesh(pMesh);
	pSpeechBar->SetComponent(pMeshRender);

	/*///////////////////////
			CMAERA
	*////////////////////////
	CObject* pObject  = new CObject();
	pObject->SetName(L"MAIN_CAMERA");
	m_pCurRoom->AddObject(LAYER_TYPE::CAMERA, pObject);

	pTrasnform = new CTransform();
	pTrasnform->SetPostion(Vector3(1.f, 1.f, 10.f));
	pObject->SetComponent(pTrasnform);

	CCamera* pCamera = new CCamera();
	pObject->SetComponent(pCamera);
}
