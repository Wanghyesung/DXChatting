#pragma once
#include "CEntity.h"

class CResource : public CEntity
{
public:
	CResource();
	virtual ~CResource();

	wstring m_strKey;
	wstring m_strRelativePath;

public:
	const wstring& GetKey() { return m_strKey; }
	const wstring& GetRelativePath() { return m_strRelativePath; }
	RESOURCE_TYPE GetType() { return m_eType; }

protected:
	void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
	void SetRelativePath(const wstring& _strPath) { m_strRelativePath = _strPath; }

protected:
	virtual int Load(const wstring& _strPath) = 0;
	virtual int Save(const wstring& _strPath) = 0;
private:
	RESOURCE_TYPE m_eType;

};

