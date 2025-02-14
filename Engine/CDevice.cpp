#include "pch.h"
#include "CDevice.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CConstBuffer.h"

CDevice::CDevice()
{

}

CDevice::~CDevice()
{
    
}

bool CDevice::init(HWND _hwnd, UINT _iWidth, UINT _iHeight)
{
    m_hwnd = _hwnd;
    m_vRenderResolution = Vec2{ _iWidth, _iHeight };

    D3D_FEATURE_LEVEL iFeatureLevels = D3D_FEATURE_LEVEL_11_0;

    IDXGIAdapter* pAdapter = nullptr;
    D3D_DRIVER_TYPE eDriverType = D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE;//Direct3D 애플리케이션에서 사용해야 하는 기본 드라이버입니다

    int iFlag = 0;
#ifdef _DEBUG
    iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

    //device 초기화
    if (FAILED(D3D11CreateDevice(pAdapter, eDriverType, nullptr, iFlag,
        nullptr, 0, D3D11_SDK_VERSION, m_device.GetAddressOf(), &iFeatureLevels,
        m_deviceContex.GetAddressOf())))
    {
        MessageBox(nullptr, L"Device, Context 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    if(FAILED(CreateSwapChain()))
    {
        MessageBox(nullptr, L"스왚체인 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }
    
    if (FAILED(CreateView()))
    {
        MessageBox(nullptr, L"View 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    //레스터라이저 단계에서 설정
    //NDC와 뷰포트의 1:1 맵핑을 보장
    m_viewPort.TopLeftX = 0.f;
    m_viewPort.TopLeftY = 0.f;
    m_viewPort.Width = m_vRenderResolution.x;
    m_viewPort.Height = m_vRenderResolution.y;

    m_viewPort.MinDepth = 0;
    m_viewPort.MaxDepth = 1.f;

    m_deviceContex->RSSetViewports(1, &m_viewPort);

    if (FAILED(CreateRasterizerState()))
    {
        MessageBox(nullptr, L"레스터라이져 스테이트 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    
    // DepthStencilState 생성
    if (FAILED(CreateDepthStencilState()))
    {
        MessageBox(nullptr, L"DepthStencilState 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    // BlendState 생성
    if (FAILED(CreateBlendState()))
    {
        MessageBox(nullptr, L"BlendState 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    // 샘플러 생성
    if (FAILED(CreateSampler()))
    {
        MessageBox(nullptr, L"샘플러 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    CreateConstBuffer();
   

    return S_OK;
}

void CDevice::Present()
{
    HRESULT hr = m_swapChain->Present(0, 0);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Present() 호출 실패!", L"Error", MB_OK);
    }
}

void CDevice::Clear()
{
    ComPtr<ID3D11RenderTargetView> RTV = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex", RESOURCE_TYPE::TEXTURE)->GetRTV();
    ComPtr<ID3D11DepthStencilView> DSV = CResMgr::GetInst()->FindRes<CTexture>(L"DepthStencil", RESOURCE_TYPE::TEXTURE)->GetDSV();

    m_deviceContex->OMSetRenderTargets(1, RTV.GetAddressOf(), DSV.Get());

    FLOAT fBackColor[4] = { 0.2f, 0.2f, 0.2f, 0.2f };
    m_deviceContex->ClearRenderTargetView(RTV.Get(), fBackColor);
    m_deviceContex->ClearDepthStencilView(DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0.f);
 
    m_deviceContex->RSSetViewports(1, &m_viewPort);
}

int CDevice::CreateSwapChain()
{
    /*
    DirectX에서 **스왑 체인(Swap Chain)**은 렌더링 결과를 저장하는 백 버퍼(back buffer)를 관리하는 구조입니다.
    스왑 체인은 CPU가 프레임을 렌더링하고 GPU가 이를 화면에 출력하는 과정에서 **더블 버퍼링(double buffering)
    */

    // 스왚체인 설정
    DXGI_SWAP_CHAIN_DESC tDesc = {};

    tDesc.OutputWindow = m_hwnd;     // 출력 윈도우
    tDesc.Windowed = true;           // 창모드, 전체화면 모드

    tDesc.BufferCount = 1; //더블 버퍼링
    tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//이 버퍼를 렌더링 타겟으로 사용
    tDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;//해상도 설정
    tDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
    tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//32비트(8비트씩 4채널) 색상 포맷 (일반적인 8비트 RGBA 색상)
    tDesc.BufferDesc.RefreshRate.Denominator = 1;//주사율(Hz)을 설정 (60Hz로 설정)
    tDesc.BufferDesc.RefreshRate.Numerator = 60;
    tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;//일반적인 방식
    tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    tDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;//이전 버퍼의 내용을 삭제

    tDesc.SampleDesc.Count = 1;//(기본값)
    tDesc.SampleDesc.Quality = 0;
    tDesc.Flags = 0;

    // 스왚체인 생성
    ComPtr<IDXGIDevice>  pDXGIDevice;
    ComPtr<IDXGIAdapter> pAdapter;
    ComPtr<IDXGIFactory> pFactory;

    HRESULT hr = S_OK;

    hr = m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
    hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
    hr = pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

    hr = pFactory->CreateSwapChain(m_device.Get(), &tDesc, m_swapChain.GetAddressOf());
    
    return hr;
}

//렌더타겟
int CDevice::CreateView()
{
    //RenderTarget
    ComPtr<ID3D11Texture2D> pTex;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pTex.GetAddressOf());
   
    shared_ptr<CTexture> pRenderTex = CResMgr::GetInst()->CreateTexture(L"RenderTargetTex", pTex);
  
    //깊이에 24비트, 스텐실에 8비트를 지원하는 32비트 z 버퍼 형식입니다.
    shared_ptr<CTexture> pDeapthTex = CResMgr::GetInst()->CreateTexture(L"DepthStencil", m_vRenderResolution.x, m_vRenderResolution.y, DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT,
        D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT);

    
    int x1 =pRenderTex->GetDesc().Width;
    int x2 = pDeapthTex->GetDesc().Width;
  

    m_deviceContex->OMSetRenderTargets(1, pRenderTex->GetRTV().GetAddressOf(), pDeapthTex->GetDSV().Get());

    return S_OK;
}

int CDevice::CreateRasterizerState()
{
    D3D11_RASTERIZER_DESC desc = {};
 
    desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;//뒷면제거
    desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;//꼭짓점에서 형성된 삼각형을 채웁니다. 인접한 꼭짓점이 그려지지 않습니다.
    m_device->CreateRasterizerState(&desc,m_RSState[(UINT)RS_TYPE::CULL_BACK].GetAddressOf());

   
    desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
    desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    m_device->CreateRasterizerState(&desc, m_RSState[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());
 
    desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    m_device->CreateRasterizerState(&desc, m_RSState[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

    desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
    m_device->CreateRasterizerState(&desc, m_RSState[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());

    return S_OK;
}

int CDevice::CreateDepthStencilState()
{
    m_DSState[(UINT)DS_TYPE::LESS] = nullptr;

    // Less Equal
    D3D11_DEPTH_STENCIL_DESC Desc = {};
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    Desc.StencilEnable = false;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    m_device->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());

    // Greater
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_GREATER;
    Desc.StencilEnable = false;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    m_device->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::GREATER].GetAddressOf());

    // Greater Equal
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
    Desc.StencilEnable = false;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    m_device->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::GREATER_EQUAL].GetAddressOf());

    // No Write
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_LESS;
    Desc.StencilEnable = false;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    m_device->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());

    // NoTest NoWrite
    Desc.DepthEnable = false;
    Desc.StencilEnable = false;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    m_device->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());

    return S_OK;
}

int CDevice::CreateBlendState()
{
    //출력 병합(Output-Merger)

    // No Blend
    m_BSState[(UINT)BS_TYPE::DEFAULT] = nullptr;

    //Src : 픽셀 셰이더에서 계산된 출력 색상
    //Dsc : 렌더 타겟에 이미 그려져 있는 기존 픽셀의 색상

    // Mask
    D3D11_BLEND_DESC Desc = {};
    Desc.AlphaToCoverageEnable = true;                  //픽셀 알파에 따라 샘플 활성 
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;            
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;  // 기본 합 연산 (Src + Dest)
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;    // 소스 색상 100%
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;  // 대상 색상 0%

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;  //알파 값도 소스만 출력.
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    m_device->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::MASK].GetAddressOf());


    // Alpha Blend
    //(SrcColor * SrcAlpha) + (DestColor * (1 - SrcAlpha))
    Desc.AlphaToCoverageEnable = false;                             // 알파 투명도 비활성화
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;              // 소스와 대상 색상의 합
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;          // 소스 알파값 사용
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;     // (1 - 소스 알파)

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;         // 알파값 합산
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;          // 소스 알파 0%
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;          // 대상 알파 100%

    //알파값 기반의 투명 효과를 구현
    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    m_device->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf());


    // One One
    //SrcColor + DestColor 1: 1 색상 혼합 : 더 밝은 효과
    Desc.AlphaToCoverageEnable = false;
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;      // 소스와 대상 색상의 합
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;        // 소스 색상 100%
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;       // 대상 색상 100%

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // 알파값 합산
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;   // 소스 알파 100%
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // 대상 알파 0%

    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    m_device->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());

    return S_OK;

}

int CDevice::CreateSampler()
{
    //텍스처를 샘플링(픽셀 데이터를 가져오는)할 때의 동작 방식
    D3D11_SAMPLER_DESC tSamDesc = {};

    //텍스처 좌표가 0~1 범위를 벗어나면 반복
    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.Filter = D3D11_FILTER_ANISOTROPIC;//텍스처를 비스듬히 보거나 확대/축소할 때 품질을 개선.
    tSamDesc.MaxLOD = D3D11_FLOAT32_MAX;
    m_device->CreateSamplerState(&tSamDesc, m_sampler[0].GetAddressOf());

    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;//텍스처를 샘플링할 때 가장 가까운 텍셀(Texel) 값을 그대로 사용.
    tSamDesc.MaxLOD = D3D11_FLOAT32_MAX;
    m_device->CreateSamplerState(&tSamDesc, m_sampler[1].GetAddressOf());


    m_deviceContex->VSSetSamplers(0, 1, m_sampler[0].GetAddressOf());
    m_deviceContex->HSSetSamplers(0, 1, m_sampler[0].GetAddressOf());
    m_deviceContex->DSSetSamplers(0, 1, m_sampler[0].GetAddressOf());
    m_deviceContex->GSSetSamplers(0, 1, m_sampler[0].GetAddressOf());
    m_deviceContex->PSSetSamplers(0, 1, m_sampler[0].GetAddressOf());
    m_deviceContex->CSSetSamplers(0, 1, m_sampler[0].GetAddressOf());
                                        
    m_deviceContex->VSSetSamplers(1, 1, m_sampler[1].GetAddressOf());
    m_deviceContex->HSSetSamplers(1, 1, m_sampler[1].GetAddressOf());
    m_deviceContex->DSSetSamplers(1, 1, m_sampler[1].GetAddressOf());
    m_deviceContex->GSSetSamplers(1, 1, m_sampler[1].GetAddressOf());
    m_deviceContex->PSSetSamplers(1, 1, m_sampler[1].GetAddressOf());
    m_deviceContex->CSSetSamplers(1, 1, m_sampler[1].GetAddressOf());

    return S_OK;
   
}

void CDevice::CreateConstBuffer()
{
    CConstBuffer* pTrConstBuffer = new CConstBuffer((UINT)CB_TYPE::TRANSFORM);
    CConstBuffer* pMtrlConstBuffer = new CConstBuffer((UINT)CB_TYPE::MATERIAL);
    CConstBuffer* pGlConstBuffer = new CConstBuffer((UINT)CB_TYPE::GLOBAL);

    m_arrConstBuffer[(UINT)CB_TYPE::TRANSFORM] = pTrConstBuffer;
    pTrConstBuffer->Create(sizeof(tTransform), 1);

    m_arrConstBuffer[(UINT)CB_TYPE::MATERIAL] = pMtrlConstBuffer;
    pMtrlConstBuffer->Create(sizeof(tMtrlConst), 1);

    m_arrConstBuffer[(UINT)CB_TYPE::GLOBAL] = pGlConstBuffer;
    pGlConstBuffer->Create(sizeof(tGlobal), 1);

}

