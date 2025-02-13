#pragma once
#include "CSingleton.h"
#include "CRoom.h"

class CObject;
class CRoomMgr : public CSingleton<CRoomMgr>
{
	friend class CSingleton;
	friend class CEngine;
	friend class CCamera;
	friend class CRenderMgr;

private:
	CRoomMgr();
	virtual ~CRoomMgr();

	CRoom* FindRoom(const wstring& _strName);
	void SetCurRoom(const wstring& _strName);
	

	const vector<CLayer*>& GetCurLayers();
	CLayer* GetCurLayer(LAYER_TYPE _eLayerType);
private:
	void tick();
	void init();
private:
	CRoom* m_pCurRoom;
	map<wstring, CRoom*>m_mapRoom;
};

