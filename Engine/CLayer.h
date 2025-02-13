#pragma once
#include "CObject.h"

class CLayer
{
	friend class CRoom;
	friend class CCamera;
	friend class CRenderMgr;
	friend class CRoomMgr;
public:
	CLayer();
	~CLayer();

private:
	void begin();
	void tick();
	void finaltick();

	CObject* FindObject(const wstring& _strName);
	void AddObject(CObject* _pObject);

private:
	vector<CObject*> m_vecObject;


};

