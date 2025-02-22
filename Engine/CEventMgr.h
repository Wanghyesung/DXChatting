#pragma once
#include "CSingleton.h"

#define MAX_FONT_COUNT 12
#define BUBBLE_SIZE_X 240
#define BUBBLE_SIZE_Y 50

class CObject;

class CEventMgr : public CSingleton<CEventMgr>
{
	friend class CSingleton;

private:
	CEventMgr();
	~CEventMgr();

public:
	void tick();

	void ChanageRoom(const WCHAR* _strPoint);
	void CreateChatting(CObject* _pObj, const WCHAR* _strPoint, bool _bOther);
	void CreateObject(CObject* _pObj, LAYER_TYPE _eType);
private:
	void excute();

private:
	Vector2 square_root_scaling(CObject* _pObj, const wstring& _strChat);
	void create_chatting(CObject* _pObj, const wstring& _strChat, bool _bOther);
	wstring adjustment_string(const wstring& _strChat);
private:
	vector<tEvent> m_vecEvent;
};

