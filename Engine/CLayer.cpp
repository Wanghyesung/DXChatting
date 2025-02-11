#include "pch.h"
#include "CLayer.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
    for (int i = 0; i < m_vecObject.size(); ++i)
    {
        delete m_vecObject[i];
        m_vecObject[i] = nullptr;
    }
}

void CLayer::begin()
{
    for (int i = 0; i < m_vecObject.size(); ++i)
    {
        m_vecObject[i]->begin();
    }
}

void CLayer::tick()
{
    for (int i = 0; i < m_vecObject.size(); ++i)
    {
        m_vecObject[i]->tick();
    }
}

void CLayer::finaltick()
{
    for (int i = 0; i < m_vecObject.size(); ++i)
    {
        m_vecObject[i]->finaltick();
    }
}

CObject* CLayer::FindObject(const wstring& _strName)
{
    for (int i = 0; i < m_vecObject.size(); ++i)
    {
        if (m_vecObject[i]->GetName() == _strName)
            return m_vecObject[i];
    }

    return nullptr;
}
