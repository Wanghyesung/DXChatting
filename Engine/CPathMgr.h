#pragma once
class CPathMgr : public CSingleton<CPathMgr>
{
	friend class CSingleton;
private:
	CPathMgr() :m_strCurrentPath{} {};
	virtual ~CPathMgr() {};

public:
	void Init();

	const wstring& GetCurrentPath() { return m_strCurrentPath; }

private:
	wstring m_strCurrentPath;

};

