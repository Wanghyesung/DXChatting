#pragma once
#include "CSingleton.h"
 
class CRenderMgr : public CSingleton<CRenderMgr>
{
	friend class CSingleton;
	friend class CEngine;
	
private:
	void render();
	void clear();

private:
	CRenderMgr();
	virtual ~CRenderMgr();
};

