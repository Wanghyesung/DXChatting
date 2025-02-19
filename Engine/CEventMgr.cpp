#include "pch.h"
#include "CEventMgr.h"
#include "CSpeechObject.h"
#include "CRoomMgr.h";
#include "CTransform.h"

CEventMgr::CEventMgr():
	m_vecEvent{}
{
}

CEventMgr::~CEventMgr()
{
}

void CEventMgr::tick()
{
	excute();
}

void CEventMgr::CreateChatting(CObject* _pObj, WCHAR* _strPoint)
{

	tEvent tEvn = {};
	tEvn.eEvent =  EVENT_TYPE::CREATE_CATTING;
	tEvn.lParam = _pObj;
	tEvn.wParam = _strPoint;
	m_vecEvent.push_back(tEvn);
	
}



void CEventMgr::excute()
{
	for (tEvent tEvn : m_vecEvent)
	{
		switch (tEvn.eEvent)
		{
		case CREATE_CATTING:
		{
			CObject* pObject = static_cast<CObject*>(tEvn.lParam);
			WCHAR* strPoint = static_cast<WCHAR*>(tEvn.wParam);
			create_chatting(pObject, strPoint);
		}
		break;
			
		default:
			break;
		}
	}
}

void CEventMgr::create_chatting(CObject* _pObj, WCHAR* _strPoint)
{
	 CTransform* pTransform = _pObj->GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);

	 //크기 조정
	 wstring strChat = wstring(_strPoint);
	 float fFontSize = dynamic_cast<CSpeechObject*>(_pObj)->GetFontSize();

	 int iSize = strChat.size();
	 int yScale = iSize / 30;
	 int xScale = 0;
	 if (yScale <= 0)
		 xScale = iSize % 30;
	 else
		 xScale = 30;


	 Vector2 vScale = Vector2{ xScale, yScale } *fFontSize;
	 Vector2 vSpawnPos = CRoomMgr::GetInst()->FindSpawnPoint(vScale);
	 pTransform->SetPostion(Vector3(vSpawnPos.x, vSpawnPos.y,-0.1f));
	 pTransform->SetScale(Vector3(vScale.x, vScale.y, 1.f));

	 CRoomMgr::GetInst()->AddObject(LAYER_TYPE::UI, _pObj);
}
