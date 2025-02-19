// ===================
// ������� ���� ����ü 1:1 ���� 
// ===================
struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;//view������ ���� ���
	Matrix matWVP;//�������� ���� ���
};


struct tMtrlConst
{
	//��Ȳ�� �°� ���
	int arrInt[4];
	float arrFloat[4];
	Vec2 arrV2[4];
	Vec4 arrV4[4];
	Matrix arrMat[4];

	//��� �ε����� �ؽ��ĸ� ���ε����� Ȯ�ο� bool int
	int arrTex[(UINT)TEX_PARAM::TEX_END];
};


struct tGlobal
{
	Vec2  Resolution;
	float tDT;
	float tAccTime;
	UINT  Light2DCount;

	int	  globalpadding[3];
};

extern tTransform g_transform;
extern tGlobal g_Global;


struct tFontInfo
{
	wstring Str;
	float fPosX; float fPosY;
	float fFontSize;
	UINT Color;
};

struct tEvent
{
	EVENT_TYPE eEvent;
	void* wParam;
	void* lParam;
};