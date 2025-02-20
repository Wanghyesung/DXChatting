#include "pch.h"
#include "CEventMgr.h"
#include "CSpeechObject.h"
#include "CRoomMgr.h";
#include "CTransform.h"
#include "CSpeechBubble.h"
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

void CEventMgr::CreateChatting(CObject* _pObj, const WCHAR* _strPoint)
{

	tEvent tEvn = {};
	tEvn.eEvent =  EVENT_TYPE::CREATE_CATTING;
	tEvn.lParam = _pObj;
	tEvn.wParam = const_cast<void*>(static_cast<const void*>(_strPoint));
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
			const WCHAR* strPoint = static_cast<const WCHAR*>(tEvn.wParam);
			create_chatting(pObject, strPoint);
		}
		break;
			
		default:
			break;
		}
	}


	m_vecEvent.clear();
}


void CEventMgr::create_chatting(CObject* _pObj, const WCHAR* _strPoint)
{
	 
	 //문자열 가공
	 wstring strChat = adjustment_string(_strPoint);
	 //크기 조정
	 Vector2 vScale = square_root_scaling(_pObj, strChat);


	 //위치
	 CTransform* pTransform = _pObj->GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);
	 Vector2 vSpawnPos = CRoomMgr::GetInst()->FindSpawnPoint(vScale);
	 vSpawnPos += (vScale / 2.f);
	 pTransform->SetPostion(Vector3(vSpawnPos.x, vSpawnPos.y,-0.1f));
	
	 CRoomMgr::GetInst()->AddObject(LAYER_TYPE::UI, _pObj);
}

Vector2 CEventMgr::square_root_scaling(CObject* _pObj, const wstring& _strChat)
{
	CTransform* pTransform = _pObj->GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);
	CSpeechBubble* pSpeechBubble = dynamic_cast<CSpeechBubble*>(_pObj);

	int iLen = _strChat.size();

	//스케일링
	float fFontSize = pSpeechBubble->GetFontSize();
	int iNewLineCnt = iLen / MAX_FONT_COUNT;
	int yScale = ((iLen - iNewLineCnt) / MAX_FONT_COUNT + 1) * BUBBLE_SIZE_Y;
	int xScale = BUBBLE_SIZE_X;

	float fBaseSize = 100;
	float fCurrentValue = yScale;
	float fMaxValue = 50 * 3.5;
	float fNewSize = fBaseSize * sqrt(fCurrentValue / fMaxValue);
	yScale = fNewSize;

	pSpeechBubble->SetSpeech(_strChat);
	Vector2 vScale = Vector2(xScale, yScale);
	pTransform->SetScale(Vector3(vScale.x, vScale.y, 1.f));

	return vScale;
}

wstring CEventMgr::adjustment_string(const WCHAR* _strPoint)
{
	int iLen = lstrlenW(_strPoint);
	wstring strChat = {};
	strChat.push_back(_strPoint[0]);

	for (int i = 1; i < iLen; ++i)
	{
		if (i % MAX_FONT_COUNT == 0)
			strChat.push_back(L'\n');

		strChat.push_back(_strPoint[i]);
	}

	return strChat;
}
