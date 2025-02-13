#include "pch.h"
#include "CResource.h"

CResource::CResource(RESOURCE_TYPE _eResType):
	m_eType(_eResType)
{
}

CResource::CResource(const CResource& _other):
	CEntity(_other),
	m_eType(_other.m_eType),
	m_strKey(_other.m_strKey),
	m_strRelativePath(_other.m_strRelativePath)
{
}

CResource::~CResource()
{
}
