#include "pch.h"
#include "CEntity.h"

UINT CEntity::GID = 1;
CEntity::CEntity():
	m_iID(GID++),
	m_strName{}
{

}

CEntity::CEntity(const CEntity& _other):
	m_iID(GID++),
	m_strName(_other.m_strName)
{
}

CEntity::~CEntity()
{
}
