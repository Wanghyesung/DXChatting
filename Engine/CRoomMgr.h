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
	friend class CUIMgr;
	friend class CUI;
public:
	Vector2 FindSpawnPoint(const Vector2& vObjectScale);
	void AddObject(LAYER_TYPE _eLayerType, CObject* _pObj);

private:
	CRoomMgr();
	virtual ~CRoomMgr();

	CRoom* FindRoom(const wstring& _strName);
	void SetCurRoom(const wstring& _strName);
	
	const vector<CLayer*>& GetCurLayers();
	CLayer* GetCurLayer(LAYER_TYPE _eLayerType);

	const vector<CObject*>& GetUIs();

	void EraseUI(CObject* _pObject);

private:
	void tick();
	void init();

private:
	CRoom* m_pCurRoom;
	map<wstring, CRoom*>m_mapRoom;

	Vector2 m_vChattingStartPos;
	float m_fChattingLine;
};

