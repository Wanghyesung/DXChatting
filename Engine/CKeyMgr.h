#pragma once
#include "CSingleton.h"


enum class KEY
{
	UP,		// VK_UP
	DOWN,
	LEFT,
	RIGHT,

	SPACE,
	ENTER,
	ESC,
	LALT,
	LCTRL,
	LSHIFT,
	TAB,

	LBTN,
	RBTN,

	A, B, C, D, E, F, G, H, I, J,
	K, L, M, N, O, P, Q, R, S, T,
	U, V, W, X, Y, Z,

	_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,

	NUM_0,NUM_1,NUM_2,NUM_3,NUM_4,NUM_5,NUM_6,NUM_7,NUM_8,NUM_9,

	BACK_SPACE,
	END,
};

enum class KEY_STATE
{
	TAP,
	PRESSED,
	RELEASE,
	NONE,
};


struct tKeyInfo
{
	KEY			key;
	KEY_STATE	state;
	bool		bPrev;
};

extern int g_arrVK[(UINT)KEY::END];

class CKeyMgr : public CSingleton<CKeyMgr>
{
	friend class CSingleton;

public:
	CKeyMgr();
	virtual ~CKeyMgr();

public:
	void init();
	void tick();

public:
	KEY_STATE GetKeyState(KEY _key) { return m_vecKey[(UINT)_key].state; }
	FORCEINLINE const Vector2& GetMousePos() { return m_vNDCMousePos; }
	Vector2 GetMouseDir() { return m_vMouseDir; }

	const unordered_map<WCHAR,bool>& GetOnKeys() { return m_hashOnKey; }

private:
	void check_key(UINT iKey);

private:
	vector<tKeyInfo> m_vecKey;
	unordered_map<WCHAR,bool> m_hashOnKey;//현재 프레임에 눌려있는 키

	Vector2 m_vMousePos;
	Vector2 m_vPrevMousePos;
	Vector2 m_vMouseDir;
	Vector2 m_vNDCMousePos;
};

