#include "pch.h"
#include "CRoom.h"

CRoom::CRoom()
{
	m_vecLayer.resize((UINT)LAYER_TYPE::END, nullptr);

	for (int i = 0; i < m_vecLayer.size(); ++i)
	{
		CLayer* pLayer = new CLayer();
		m_vecLayer[i] = pLayer;
	}
}

CRoom::~CRoom()
{
	for (int i = 0; i < m_vecLayer.size(); ++i)
	{
		delete m_vecLayer[i];
		m_vecLayer[i] = nullptr;
	}
}

void CRoom::begin()
{
	for (int i = 0; i < m_vecLayer.size(); ++i)
	{
		m_vecLayer[i]->begin();
	}
}

void CRoom::tick()
{
	for (int i = 0; i < m_vecLayer.size(); ++i)
	{
		m_vecLayer[i]->tick();
	}
}

void CRoom::finaltick()
{
	for (int i = 0; i < m_vecLayer.size(); ++i)
	{
		m_vecLayer[i]->finaltick();
	}
}

void CRoom::AddObject(LAYER_TYPE _eType, CObject* _pObject)
{
	m_vecLayer[(UINT)_eType]->AddObject(_pObject);
}


