#pragma once
class CEntity
{
public:
	CEntity();
	CEntity(const CEntity& _other);
	~CEntity();

	const wstring& GetName() { return m_strName; }
	UINT GetID() { return m_iID; }


	void SetName(const wstring& _strName) { m_strName = _strName; }
private:
	wstring m_strName;
	UINT m_iID;

	static UINT GID;
};

