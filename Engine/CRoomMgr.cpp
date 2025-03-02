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

Vector2 CRoomMgr::FindSpawnPoint(const Vector2& vObjectScale, bool _bOther)
{
	//lock
	CLayer* pLayer = GetCurLayer(LAYER_TYPE::CAMERA);
	CObject* pCamera = pLayer->m_vecObject[0];

	CTransform* pTransform = pCamera->GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);
	Vector3 vCameraPos = pTransform->GetPosition();
	//���ؼ� ������ �����ٸ� �� ī�޶� ��ġ ������

	Vector2 vRetPos = Vector2::Zero;
	m_vChattingStartPos.y -= (vObjectScale.y + 10.f);
	vRetPos = m_vChattingStartPos;
	if (_bOther)
		vRetPos.x *= -1;

	if (m_vChattingStartPos.y <= m_fChattingLine)
	{
		//ī�޶�� ���� ���� ������
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

void CRoomMgr::AddObject(const wstring& _strRoomName, LAYER_TYPE _eLayerType, CObject* _pObj)
{
	auto iter = m_mapRoom.find(_strRoomName);
	if (iter == m_mapRoom.end())
		return;

	iter->second->AddObject(_eLayerType, _pObj);
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
	//������ ��Ŷ ����
	//�α׸� ��Ŷ ���� ������ �������� ����� session recv���� Login�Լ��� 
	CEventMgr::GetInst()->ChanageRoom(L"Chatting");

	if (m_strClientName.size() == 0)
		assert(nullptr);

	GPersonList->add_propile(m_strClientName);
}

void CRoomMgr::AddProPile(const wstring& _strName)
{
	GPersonList->add_propile(_strName);
}

void CRoomMgr::EraseProPile(const wstring& _strName)
{
	GPersonList->erase_propile(_strName);
}


CObject* CRoomMgr::FindParentObject(const wstring& _strName, LAYER_TYPE _eLayer)
{
	vector<CObject*>& vecObj = m_pCurRoom->m_vecLayer[(UINT)_eLayer]->m_vecObject;

	vector<CObject*>::iterator iter = vecObj.begin();
	for (iter; iter != vecObj.end(); ++iter)
	{
		if ((*iter)->GetName() == _strName)
		{
			return *iter;
		}
	}

	return nullptr;
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
	

	/*///////////////////////
			CMAERA
	*////////////////////////
	CObject* pObject  = new CObject();
	pObject->SetName(L"LOBBY_CAMERA");
	pLobby->AddObject(LAYER_TYPE::CAMERA, pObject);

	CTransform* pTrasnform = new CTransform();
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

	shared_ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ChattingRoomMaterial", RESOURCE_TYPE::MATERIAL);
	shared_ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	CMeshRender* pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
	pMeshRender->SetMesh(pMesh);
	pObject->SetComponent(pMeshRender);

	//SpeechBar


	//list
	GPersonList = new CPersonList();
	pChattingRoom->AddObject(LAYER_TYPE::UI, GPersonList);
	GPersonList->SetName(L"PsersonList");
	
	pTrasnform = new CTransform();
	pTrasnform->SetPostion(Vector3{-450.f,0.f,-0.2f });
	GPersonList->SetStaticPos(Vector3{ -450.f,0.f,-0.2f });
	pTrasnform->SetScale(Vector3{ 300.f,500.f,1.f });
	GPersonList->SetComponent(pTrasnform);

	pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ListMaterial", RESOURCE_TYPE::MATERIAL);
	pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh", RESOURCE_TYPE::MESH);
	pMeshRender = new CMeshRender();
	pMeshRender->SetMaterial(pMtrl);
	pMeshRender->SetMesh(pMesh);
	GPersonList->SetComponent(pMeshRender);
	//GPersonList->add_propile(L"Me");
	//GPersonList->add_propile(L"������");
	//GPersonList->add_propile(L"������������������");
	//GPersonList->add_propile(L"��������");
	//GPersonList->add_propile(L"������");
}
