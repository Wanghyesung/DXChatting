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

void CEventMgr::CreateChatting(CObject* _pObj, const WCHAR* _strPoint, bool _bOther)
{
	tEvent tEvn = {};
	tEvn.eEvent =  EVENT_TYPE::CREATE_CATTING;
	tEvn.lParam = _pObj; //�̹� new�� �Ҵ��� ��ü�� delete�� 2���� ������ ���� (shared_ptr<class>(_pObj);)
	tEvn.wParam = make_shared<wstring>(_strPoint); //_strPoint�� ���� ������ ���ο� std::wstring ��ü�� ��(Heap) �޸𸮿� ����
	// const_cast<void*>(static_cast<const void*>(_strPoint));
	tEvn.xParam = make_shared<bool>(_bOther);

	m_vecEvent.push_back(tEvn);
}

void CEventMgr::CreateObject(CObject* _pObj, LAYER_TYPE _eType)
{
	tEvent tEvn = {};
	tEvn.eEvent = EVENT_TYPE::CREATE_OBJECT;
	tEvn.eLayer = _eType;
	tEvn.lParam = _pObj;

	m_vecEvent.push_back(tEvn);
}




void CEventMgr::excute()
{
	for (tEvent tEvn : m_vecEvent)
	{
		switch (tEvn.eEvent)
		{
		case CREATE_OBJECT:
		{
			CObject* pObject = static_cast<CObject*>(tEvn.lParam);
			LAYER_TYPE eLayerType = tEvn.eLayer;

			CRoomMgr::GetInst()->AddObject(eLayerType, pObject);
		}
		break;

		case CREATE_CATTING:
		{
			CObject* pObject = static_cast<CObject*>(tEvn.lParam); 
			const wstring& strChat = *static_cast<wstring*>(tEvn.wParam.get()); //�����簡 ���� ������ �ޱ�
			bool bOther = *static_cast<bool*>(tEvn.xParam.get());
			create_chatting(pObject, strChat, bOther);
		}
		break;
		

		default:
			break;
		}
	}


	m_vecEvent.clear();
}


void CEventMgr::create_chatting(CObject* _pObj, const wstring& _strChat, bool _bOther)
{
	 //���ڿ� ����
	 wstring strChat = adjustment_string(_strChat);
	 //ũ�� ����
	 Vector2 vScale = square_root_scaling(_pObj, strChat);

	 //��ġ
	 CTransform* pTransform = _pObj->GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);
	 Vector2 vSpawnPos = CRoomMgr::GetInst()->FindSpawnPoint(vScale, _bOther);
	 vSpawnPos += (vScale / 2.f);
	 pTransform->SetPostion(Vector3(vSpawnPos.x, vSpawnPos.y,-0.1f));
	
	 CRoomMgr::GetInst()->AddObject(LAYER_TYPE::UI, _pObj);
}


Vector2 CEventMgr::square_root_scaling(CObject* _pObj, const wstring& _strChat)
{
	CTransform* pTransform = _pObj->GetComponent<CTransform>(COMPONENT_TYPE::TRANSFORM);
	CSpeechBubble* pSpeechBubble = dynamic_cast<CSpeechBubble*>(_pObj);

	int iLen = _strChat.size();

	//�����ϸ�
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

wstring CEventMgr::adjustment_string(const wstring& _strChat)
{
	int iLen = _strChat.size();
	wstring strChat = {};
	strChat.push_back(_strChat[0]);

	for (int i = 1; i < iLen; ++i)
	{
		if (i % MAX_FONT_COUNT == 0)
			strChat.push_back(L'\n');

		strChat.push_back(_strChat[i]);
	}

	return strChat;
}
