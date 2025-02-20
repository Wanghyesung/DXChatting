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
	void* lParam;
	shared_ptr<void> wParam;
	
	//���� ������ �ѱ�� ���� �����ӿ��� ���� �����Ͱ� ������� ������ shared�� ����
	shared_ptr<void> xParam;
	
	//std::unordered_map<std::type_index, std::shared_ptr<void>> xParam;
	//template <typename T>
	//void SetParam(shared_ptr<T> _ptr)
	//{
	//	xParam[typeid(T)] = _ptr;
	//}
	//
	//template <typename T>
	//shared_ptr<T> GetParam()
	//{
	//	return static_pointer_cast<T>(xParam[typeid(T)]);
	//}
};