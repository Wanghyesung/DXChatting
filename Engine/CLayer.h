#pragma once
#include "CObject.h"

class CLayer
{
	friend class CRoom;
	friend class CCamera;
public:
	CLayer();
	~CLayer();

private:
	void begin();
	void tick();
	void finaltick();

	CObject* FindObject(const wstring& _strName);

private:
	vector<CObject*> m_vecObject;


};

