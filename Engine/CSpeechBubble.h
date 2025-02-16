#pragma once

#include "CUI.h"

class CSpeechBubble : public CUI
{
public:
	CSpeechBubble();
	virtual ~CSpeechBubble();


private:
	wstring m_strSpeech;
	CSpeechBubble* m_pSpeechTarget;

};

