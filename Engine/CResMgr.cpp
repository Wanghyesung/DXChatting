#include "pch.h"
#include "CResMgr.h"
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"

CResMgr::CResMgr():
	m_iLayoutOffset(0)
{

}

CResMgr::~CResMgr()
{

}

void CResMgr::Init()
{
	InitMesh();
	InitShader();
	InitMaterial();
	InitTexture();
}

void CResMgr::InitMesh()
{
	vector<tVertex> vecVtx;
	vector<UINT> vecIdx;

	tVertex v;


	shared_ptr<CMesh> pMesh = nullptr;

	// ==============
	//	  PointMesh
	// ==============
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);

	UINT idx = 0;

	pMesh = make_shared<CMesh>();
	pMesh->Create(&v, 1, &idx, 1);
	AddRes<CMesh>(L"PointMesh", pMesh, RESOURCE_TYPE::MESH);


	// =============
	//	 RectMesh
	// =============
	//	 0 --- 1 
	//	 |  \  |
	//	 3 --- 2
	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = make_shared<CMesh>();
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"RectMesh", pMesh, RESOURCE_TYPE::MESH);
}

void CResMgr::InitShader()
{
	shared_ptr<CGraphicsShader> pShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	AddRes<CShader>(L"std2dShader", pShader, RESOURCE_TYPE::SHADER);
}


void CResMgr::InitMaterial()
{
	shared_ptr<CMaterial> pDefaultMaterial;
	pDefaultMaterial->SetShader(FindRes<CGraphicsShader>(L"std2dShader", RESOURCE_TYPE::SHADER));
	AddRes<CMaterial>(L"DefaultMaterial", pDefaultMaterial, RESOURCE_TYPE::MATERIAL);
}

void CResMgr::InitTexture()
{
	shared_ptr<CTexture> pTexture;
    pTexture->Load(L"Texture\\tem");
	AddRes<CTexture>(L"TemTex", pTexture, RESOURCE_TYPE::TEXTURE);
}

void CResMgr::AddInputLayout(DXGI_FORMAT _eFormat, const char* _strSemanticName, UINT _iSlotNum, UINT _iSemanticIdx)
{
	//입력 레이아웃(Input Layout)을 구성

	D3D11_INPUT_ELEMENT_DESC desc = {};
	desc.Format = _eFormat;								//타입을 지정
	desc.SemanticName = _strSemanticName;				//HLSL 셰이더에서 사용하는 변수 명
	desc.InputSlot = _iSlotNum;							//여러 개의 정점 버퍼를 사용할 때 사용
	desc.SemanticIndex = _iSemanticIdx;					//TEXCOORD0  // 첫 번째 UV , TEXCOORD1  // 두 번째 UV

	desc.InstanceDataStepRate = 0;						//인스턴싱 데이터 X
	desc.AlignedByteOffset = m_iLayoutOffset;			//현재 속성(Attribute)의 바이트 오프셋을 지정
	desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;  //정점 데이터(Vertex Data)인지, 인스턴싱 데이터(Instance Data)인지 
	
	m_vecInputDesc.push_back(desc);

	m_iLayoutOffset += GetSizeofFormat(_eFormat);		//정점 속성 offset 더하기
}

std::shared_ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _pixelformat, UINT _iBindFlag, D3D11_USAGE _Usage)
{
	shared_ptr<CTexture> pTex = make_shared<CTexture>();
	
	assert(FindRes<CTexture>(_strKey, RESOURCE_TYPE::TEXTURE) == nullptr);

	if (FAILED(pTex->CreateTex(_iWidth, _iHeight, _pixelformat, _iBindFlag, _Usage)))
	{
		assert(nullptr);
	}
	
	AddRes<CTexture>(_strKey, pTex, RESOURCE_TYPE::TEXTURE);

	return pTex;
}

std::shared_ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
	shared_ptr<CTexture> pTex = make_shared<CTexture>();

	assert(FindRes<CTexture>(_strKey, RESOURCE_TYPE::TEXTURE) == nullptr);

	if (FAILED(pTex->CreateTex(_Tex2D)))
	{
		assert(nullptr);
	}

	AddRes<CTexture>(_strKey, pTex, RESOURCE_TYPE::TEXTURE);

	return pTex;
}
