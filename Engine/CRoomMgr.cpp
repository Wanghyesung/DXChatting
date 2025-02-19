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
#include "CEngine.h"


CRoomMgr::CRoomMgr() :
	m_pCurRoom(nullptr),
	m_vChattingStartPos(Vector2::Zero)
{
	m_vChattingStartPos = Vector2{ -200.f, 300.f };
	m_fChattingLine = -250.f;

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

Vector2 CRoomMgr::FindSpawnPoint(const Vector2& vObjectScale)
{
	CLayer* pLayer = GetCurLayer(LAYER_TYPE::CAMERA);
	CObject* pCamera = pLayer->m_vecObject[0];

	CTransform* pTransform = pCamera->GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);
	Vector3 vCameraPos = pTransform->GetPosition();
	//기준점 밖으로 나갔다면 내 카메라 위치 내리기

	m_vChattingStartPos.y += vObjectScale.y;
	if (m_vChattingStartPos.y >= m_fChattingLine)
	{
		float fDiff = m_vChattingStartPos.y - m_fChattingLine;
		m_fChattingLine += m_vChattingStartPos.y;

		vCameraPos.y += fDiff;
		pTransform->SetPostion(vCameraPos);
	}

	return m_vChattingStartPos;
}

void CRoomMgr::AddObject(LAYER_TYPE _eLayerType, CObject* _pObj)
{
	m_pCurRoom->AddObject(_eLayerType, _pObj);
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
	CRoom* pChattingRoom = new CRoom();
	m_mapRoom.insert(make_pair(L"Chatting", m_pCurRoom));

	CRoom* pLobby = new CRoom();
	m_mapRoom.insert(make_pair(L"lobby", pLobby));
	m_pCurRoom = pChattingRoom;

	/*//////////////
		 Lobby
	*///////////////
	CSpeechObject* pUI = new CSpeechObject();
	pUI->SetName(L"TemObject");
	pUI->SetSpeech(L"채팅방 참가");
	
	pUI->SetFontSize(50.f);
	
	pLobby->AddObject(LAYER_TYPE::UI, pUI);
	
	CTransform* pTrasnform = new CTransform();
	pTrasnform->SetPostion(Vector3{0.f,100.f,-0.2f});
	pTrasnform->SetScale(Vector3{ 300.f,100.f,1.f });
	pUI->SetComponent(pTrasnform);


	shared_ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DefaultMaterial", RESOURCE_TYPE::MATERIAL);
	shared_ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	CMeshRender* pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
	pMeshRender->SetMesh(pMesh);
	//pMtrl->SetTex(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"TemTex", RESOURCE_TYPE::TEXTURE));
	pUI->SetComponent(pMeshRender);

	/*///////////////////////
			SPEECHBAR
	*////////////////////////
	CSpeechBar* pSpeechBar = new CSpeechBar();
	pUI->SetName(L"SpeechBar");
	pLobby->AddObject(LAYER_TYPE::UI, pSpeechBar);

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
	pObject->SetName(L"LOBBY_CAMERA");
	pLobby->AddObject(LAYER_TYPE::CAMERA, pObject);

	pTrasnform = new CTransform();
	pTrasnform->SetPostion(Vector3(1.f, 1.f, 10.f));
	pObject->SetComponent(pTrasnform);

	CCamera* pCamera = new CCamera();
	pObject->SetComponent(pCamera);


	/*//////////////
		 Chatting
	*///////////////

	pObject = new CObject();
	pObject->SetName(L"CHATTING_CAMERA");
	pChattingRoom->AddObject(LAYER_TYPE::CAMERA, pObject);

	pTrasnform = new CTransform();
	pTrasnform->SetPostion(Vector3(1.f, 1.f, 10.f));
	pObject->SetComponent(pTrasnform);

	pCamera = new CCamera();
	pObject->SetComponent(pCamera);

	//SpeechBar
	pSpeechBar = new CSpeechBar();
	pChattingRoom->AddObject(LAYER_TYPE::UI, pSpeechBar);

	pTrasnform = new CTransform();
	pTrasnform->SetPostion(Vector3{ 100.f,-300.f,-0.2f });
	pTrasnform->SetScale(Vector3{ 500.f,50.f,1.f });
	pSpeechBar->SetComponent(pTrasnform);


	pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DefaultMaterial", RESOURCE_TYPE::MATERIAL);
	pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
	pDynamicMtrl = pMeshRender->GetDynamicMaterial();
	pDynamicMtrl->SetTex(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"SpeechBarTex", RESOURCE_TYPE::TEXTURE));
	pMeshRender->SetMaterial(pDynamicMtrl);

	pMeshRender->SetMesh(pMesh);
	pSpeechBar->SetComponent(pMeshRender);


}
