#include "pch.h"
#include "CRoom.h"

CRoom::CRoom()
{
	m_vecLayer.resize((UINT)LAYER_TYPE::END);
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
