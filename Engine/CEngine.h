#pragma once
#include "CSingleton.h"


class CEngine : public CSingleton<CEngine>
{
	friend class CSingleton;

private:
	CEngine();
	virtual ~CEngine();

public:
	Vector2 GetWindowResolution() { return m_vResolution; }
	HWND GetMainWnd() { return m_hWnd; }

public:
	int init(HWND _hWnd, UINT _iWidth, UINT _iHeight);
	int init_mgr();

	void progress();

private:
	void tick();
	void render();
	

private:
	HWND m_hWnd;

	Vector2 m_vResolution;


};

