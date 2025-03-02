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
	Vector2 FindSpawnPoint(const Vector2& vObjectScale, bool _bOther = false);
	float GetUIOffset() { return m_fUIOffset; }

	CObject* FindParentObject(const wstring& _strName, LAYER_TYPE _eLayer);
	void EraseObject(CObject* _pObject, LAYER_TYPE _eLayer); 
	
	void SetClientName(const wstring& _strName) { m_strClientName = _strName; }
	const wstring& GetClientName() { return m_strClientName; }

	void ClientLogin();
	void AddProPile(const wstring& _strName);
	void EraseProPile(const wstring& _strName);

	void AddObject(LAYER_TYPE _eLayerType, CObject* _pObj);
	void AddObject(const wstring& _strRoomName, LAYER_TYPE _eLayerType, CObject* _pObj);

private:
	CRoomMgr();
	virtual ~CRoomMgr();

	void ChanageRoom(const wstring& _strName);

	CRoom* FindRoom(const wstring& _strName);
	void SetCurRoom(const wstring& _strName);
	
	const vector<CLayer*>& GetCurLayers();
	CLayer* GetCurLayer(LAYER_TYPE _eLayerType);

	const vector<CObject*>& GetUIs();


	//recv로 받으면 ICOP에서 함수로 받아서 씬 이동

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

	wstring m_strClientName;

};

