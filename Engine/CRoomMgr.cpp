#include "pch.h"
#include "CRoomMgr.h"

CRoomMgr::CRoomMgr() :
	m_pCurRoom(nullptr)
{
}

CRoomMgr::~CRoomMgr()
{
	auto iter = m_mapRoom.begin();
	for (iter; iter != m_mapRoom.end(); ++iter)
	{
		delete iter->second;
		iter->second = nullptr;
	}
	m_mapRoom.clear();
}


CRoom* CRoomMgr::FindRoom(const wstring& _strName)
{
	auto iter = m_mapRoom.find(_strName);
	if (iter != m_mapRoom.end())
		return iter->second;

	return nullptr;
}

void CRoomMgr::SetCurRoom(const wstring& _strName)
{
	CRoom* pRoom = FindRoom(_strName);
	if (pRoom == nullptr)
		assert(nullptr);

	m_pCurRoom = pRoom;
}

const vector<CLayer*>& CRoomMgr::GetCurLayers()
{
	if (m_pCurRoom == nullptr)
		assert(nullptr);

	return m_pCurRoom->m_vecLayer;
}


void CRoomMgr::tick()
{
	m_pCurRoom->tick();

	m_pCurRoom->finaltick();
}
