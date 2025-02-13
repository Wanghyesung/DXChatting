#pragma once
#include "CLayer.h"

class CRoom
{
	friend class CRoomMgr;
public:
	CRoom();
	virtual ~CRoom();
	
	void begin();
	void tick();
	void finaltick();

private:
	CLayer* GetLayer(LAYER_TYPE _eType) { return m_vecLayer[(UINT)_eType];}
	void AddObject(LAYER_TYPE _eType, CObject* _pObject);
private:
	vector<CLayer*> m_vecLayer;

};

