#pragma once
#include "CSingleton.h"

class CObject;

class CEventMgr : public CSingleton<CEventMgr>
{
	friend class CSingleton;

private:
	CEventMgr();
	~CEventMgr();

public:
	void tick();

	void CreateChatting(CObject* _pObj, WCHAR* _strPoint);
private:
	void excute();

	void create_chatting(CObject* _pObj,  WCHAR* _strPoint);
	//void add_event(EVENT_TYPE _eEventType, void* _pLparam, void* _pWparam);
private:
	vector<tEvent> m_vecEvent;
};

