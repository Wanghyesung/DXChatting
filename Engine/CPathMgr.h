#pragma once
class CPathMgr : public CSingleton<CPathMgr>
{
	friend class CSingleton;
private:
	CPathMgr() :m_strCurrentPath{} {};
	virtual ~CPathMgr() {};

public:
	void Init();

	const WCHAR* GetCurrentPath() { return m_strCurrentPath; }

private:
	WCHAR m_strCurrentPath[256];

};

