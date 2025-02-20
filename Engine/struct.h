// ===================
// 상수버퍼 대응 구조체 1:1 맞춤 
// ===================
struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;//view까지만 구한 행렬
	Matrix matWVP;//투영까지 구한 행렬
};


struct tMtrlConst
{
	//상황에 맞게 사용
	int arrInt[4];
	float arrFloat[4];
	Vec2 arrV2[4];
	Vec4 arrV4[4];
	Matrix arrMat[4];

	//몇번 인덱스에 텍스쳐를 바인딩할지 확인용 bool int
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
	
	//지역 변수로 넘길시 다음 프레임에서 원본 데이터가 사라지게 때문에 shared로 저장
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