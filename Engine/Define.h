
#define KEY_TAP(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::TAP		
#define KEY_RELEASE(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::RELEASE
#define KEY_PRESSED(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::PRESSED

enum class RESOURCE_TYPE
{
	MESH,			// 형태
	TEXTURE,		// 이미지
	SHADER,
	MATERIAL,
	END,
};

enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END,
};

enum class BS_TYPE
{
	DEFAULT,		// No Blending
	MASK,			// Alpha Coverage
	ALPHA_BLEND,	// Alpha 계수 
	ONE_ONE,		// 1:1 혼합

	END,
};

enum class DS_TYPE
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	NO_WRITE,			// LESS, DepthWrite X
	NO_TEST_NO_WRITE,	// Test X, DepthWrite X
	END,
};

enum class CB_TYPE
{
	TRANSFORM,
	MATERIAL,
	GLOBAL,
	END,
};


struct tVertex
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,

	TEX_END,
};

enum SHADER_FLAG
{
	VS = 1 << 0,  
	HS = 1 << 1,  
	DS = 1 << 2,  
	GS = 1 << 3,  
	PS = 1 << 4,  
	ALL = VS | HS | DS | GS | PS ,
};

enum MTRL_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
};

enum COMPONENT_TYPE
{
	TRANSFORM,
	ANIMATOR,
	COLLISION,

	CAMERA,

	MESH_RENDER,
	END,
};

enum BUFFER_TYPE
{
	READ,
	WRITE,
};

enum class LAYER_TYPE
{
	PLAYER,
	BUBBLE,
	CAMERA,
	UI,
	END,
};

enum class PROJ_TYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE,
};

enum class RENDER_TYPE
{
	R_OPAQUE,
	R_TRANSLUCENT,
	R_TRANSPARENT,
};

enum EVENT_TYPE
{
	CREATE_CATTING,
	CTEATE_OTHER_CAHTTING,
};