
enum class RESOURCE_TYPE
{
	MESH,			// 형태
	TEXTURE,		// 이미지
	
	SHADER,

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