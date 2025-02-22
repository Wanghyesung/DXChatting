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
	friend class CEventMgr;

public:
	Vector2 FindSpawnPoint(const Vector2& vObjectScale, bool _bOtehr = false);
	float GetUIOffset() { return m_fUIOffset; }

	void EraseObject(CObject* _pObject, LAYER_TYPE _eLayer); //todo :EventMgr에서 처리하게

private:
	CRoomMgr();
	virtual ~CRoomMgr();

	void AddObject(LAYER_TYPE _eLayerType, CObject* _pObj);

	CRoom* FindRoom(const wstring& _strName);
	void SetCurRoom(const wstring& _strName);
	
	const vector<CLayer*>& GetCurLayers();
	CLayer* GetCurLayer(LAYER_TYPE _eLayerType);

	const vector<CObject*>& GetUIs();
private:
	void tick();
	void init();

private:
	CRoom* m_pCurRoom;
	map<wstring, CRoom*>m_mapRoom;


	static class CPersonList* GPersonList;
	Vector2 m_vChattingStartPos;
	float m_fChattingLine;
	float m_fUIOffset;
};

