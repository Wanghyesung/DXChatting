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
#include "CPersonList.h"
#include "CEventMgr.h"
#include "CLoginUI.h"

CPersonList* CRoomMgr::GPersonList = nullptr;

CRoomMgr::CRoomMgr() :
	m_pCurRoom(nullptr),
	m_vChattingStartPos(Vector2::Zero),
	m_fUIOffset(0.f)
{
	m_vChattingStartPos = Vector2{ -170.f, 250.f };
	m_fChattingLine = -230.f;

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

void CRoomMgr::ChanageRoom(const wstring& _strName)
{
	auto iter = m_mapRoom.find(_strName);
	if (iter != m_mapRoom.end())
	{
		//exit
		m_pCurRoom = iter->second;
		//enter
	}
}

Vector2 CRoomMgr::FindSpawnPoint(const Vector2& vObjectScale, bool _bOtehr)
{
	//lock
	CLayer* pLayer = GetCurLayer(LAYER_TYPE::CAMERA);
	CObject* pCamera = pLayer->m_vecObject[0];

	CTransform* pTransform = pCamera->GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);
	Vector3 vCameraPos = pTransform->GetPosition();
	//기준점 밖으로 나갔다면 내 카메라 위치 내리기

	Vector2 vRetPos = Vector2::Zero;
	m_vChattingStartPos.y -= (vObjectScale.y + 10.f);
	vRetPos = m_vChattingStartPos;
	if (_bOtehr)
		vRetPos.x *= -1;

	if (m_vChattingStartPos.y <= m_fChattingLine)
	{
		//카메라와 같이 라인 내리기
		m_fChattingLine -= (vObjectScale.y + 10.f);
		m_fUIOffset -= (vObjectScale.y + 10.f);

		vCameraPos.y -= (vObjectScale.y + 10.f);
		pTransform->SetPostion(vCameraPos);
	}

	return vRetPos;
}

void CRoomMgr::AddObject(LAYER_TYPE _eLayerType, CObject* _pObj)
{
	m_pCurRoom->AddObject(_eLayerType, _pObj);
	_pObj->SetLayer(_eLayerType);
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

void CRoomMgr::ClientLogin()
{
	//서버에 패킷 전송

	Login(); //나중에 session recv에서 처리
}

void CRoomMgr::Login()
{
	//로그린 패킷 먼저 보내고 서버에서 허락후 session recv에서 Login함수로 
	CEventMgr::GetInst()->ChanageRoom(L"Chatting");

	if (m_strClientName.size() == 0)
		assert(nullptr);

	GPersonList->add_propile(m_strClientName);
}

void CRoomMgr::EraseObject(CObject* _pObject, LAYER_TYPE _eLayer)
{
	vector<CObject*>& vecObj = m_pCurRoom->m_vecLayer[(UINT)_eLayer]->m_vecObject;
	
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
	m_mapRoom.insert(make_pair(L"Chatting", pChattingRoom));

	CRoom* pLobby = new CRoom();
	m_mapRoom.insert(make_pair(L"lobby", pLobby));
	m_pCurRoom = pLobby;

	/*//////////////
		 Lobby
	*///////////////
	CLoginUI* pUI = new CLoginUI();
	pUI->SetName(L"TemObject");
	pUI->SetSpeech(L"채팅방 참가");
	pUI->SetFontSize(50.f);

	//bind 함수를 "고정된 인자"와 함께 저장하여 나중에 실행할 수 있도록 만드는 것.
	pUI->SetMouseClickedFunction(std::bind(&CRoomMgr::ClientLogin, this)); //콜백함수

	pLobby->AddObject(LAYER_TYPE::UI, pUI);
	
	CTransform* pTrasnform = new CTransform();
	pTrasnform->SetPostion(Vector3{0.f,100.f,-0.2f});
	pTrasnform->SetScale(Vector3{ 300.f,100.f,1.f });
	pUI->SetComponent(pTrasnform);


	shared_ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"BorderMaterial", RESOURCE_TYPE::MATERIAL);
	shared_ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	CMeshRender* pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
	pMeshRender->SetMesh(pMesh);
	pUI->SetComponent(pMeshRender);

	/*///////////////////////
			SPEECHBAR
	*////////////////////////
	CSpeechBar* pSpeechBar = new CSpeechBar();
	pUI->SetSpeechBar(pSpeechBar);
	pSpeechBar->SetBaseSpeech(L"NAME :");
	pSpeechBar->SetName(L"SpeechBar");
	pLobby->AddObject(LAYER_TYPE::UI, pSpeechBar);
	pSpeechBar->SetFontSize(40.f);

	pTrasnform = new CTransform();
	pSpeechBar->SetStaticPos(Vector3{ 0.f,-100.f,-0.2f });
	pTrasnform->SetScale(Vector3{ 400.f,100.f,1.f });
	pSpeechBar->SetComponent(pTrasnform);

	pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"BorderMaterial", RESOURCE_TYPE::MATERIAL);
	pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
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

	//back
	pObject = new CObject();
	pObject->SetName(L"CAHTTING_BACK");
	pChattingRoom->AddObject(LAYER_TYPE::BASE, pObject);
	
	pTrasnform = new CTransform();
	pTrasnform->SetPostion(Vector3(1.f, 1.f, -0.4f));
	pObject->SetComponent(pTrasnform);

	pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ChattingRoomMaterial", RESOURCE_TYPE::MATERIAL);
	pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
	pMeshRender->SetMesh(pMesh);
	pObject->SetComponent(pMeshRender);

	//SpeechBar
	pSpeechBar = new CSpeechBar();
	pChattingRoom->AddObject(LAYER_TYPE::UI, pSpeechBar);

	pTrasnform = new CTransform();
	pSpeechBar->SetStaticPos(Vector3{ 100.f,-300.f,-0.2f });
	pTrasnform->SetPostion(Vector3{ 100.f,-300.f,-0.2f });
	pTrasnform->SetScale(Vector3{ 500.f,50.f,1.f });
	pSpeechBar->SetComponent(pTrasnform);


	pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DefaultMaterial", RESOURCE_TYPE::MATERIAL);
	pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
	shared_ptr<CMaterial> pDynamicMtrl = pMeshRender->GetDynamicMaterial();
	pDynamicMtrl->SetTex(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"SpeechBarTex", RESOURCE_TYPE::TEXTURE));
	pMeshRender->SetMaterial(pDynamicMtrl);

	pMeshRender->SetMesh(pMesh);
	pSpeechBar->SetComponent(pMeshRender);


	//list
	GPersonList = new CPersonList();
	pChattingRoom->AddObject(LAYER_TYPE::UI, GPersonList);
	GPersonList->SetName(L"PsersonList");
	
	pTrasnform = new CTransform();
	pTrasnform->SetPostion(Vector3{-450.f,0.f,-0.2f });
	pTrasnform->SetScale(Vector3{ 300.f,500.f,1.f });
	GPersonList->SetComponent(pTrasnform);

	pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ListMaterial", RESOURCE_TYPE::MATERIAL);
	pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
	pMeshRender->SetMesh(pMesh);
	GPersonList->SetComponent(pMeshRender);
	//GPersonList->add_propile(L"Me");
	//GPersonList->add_propile(L"왕혜성");
	//GPersonList->add_propile(L"ㅇㄴㄹㄷㅈㄹㅈㄷㄹ");
	//GPersonList->add_propile(L"ㅇㄴㄹㄷ");
	//GPersonList->add_propile(L"ㅇㄴㄹ");
}
