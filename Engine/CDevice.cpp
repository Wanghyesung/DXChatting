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
    D3D_DRIVER_TYPE eDriverType = D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE;//Direct3D ���ø����̼ǿ��� ����ؾ� �ϴ� �⺻ ����̹��Դϴ�

    int iFlag = 0;
#ifdef _DEBUG
    iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

    //device �ʱ�ȭ
    if (FAILED(D3D11CreateDevice(pAdapter, eDriverType, nullptr, iFlag,
        nullptr, 0, D3D11_SDK_VERSION, m_device.GetAddressOf(), &iFeatureLevels,
        m_deviceContex.GetAddressOf())))
    {
        MessageBox(nullptr, L"Device, Context ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
        return E_FAIL;
    }

    if(FAILED(CreateSwapChain()))
    {
        MessageBox(nullptr, L"����ü�� ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
        return E_FAIL;
    }
    
    if (FAILED(CreateView()))
    {
        MessageBox(nullptr, L"View ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
        return E_FAIL;
    }

    //�����Ͷ����� �ܰ迡�� ����
    //NDC�� ����Ʈ�� 1:1 ������ ����
    m_viewPort.TopLeftX = 0.f;
    m_viewPort.TopLeftY = 0.f;
    m_viewPort.Width = m_vRenderResolution.x;
    m_viewPort.Height = m_vRenderResolution.y;

    m_viewPort.MinDepth = 0;
    m_viewPort.MaxDepth = 1.f;

    m_deviceContex->RSSetViewports(1, &m_viewPort);

    if (FAILED(CreateRasterizerState()))
    {
        MessageBox(nullptr, L"�����Ͷ����� ������Ʈ ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
        return E_FAIL;
    }

    
    // DepthStencilState ����
    if (FAILED(CreateDepthStencilState()))
    {
        MessageBox(nullptr, L"DepthStencilState ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
        return E_FAIL;
    }

    // BlendState ����
    if (FAILED(CreateBlendState()))
    {
        MessageBox(nullptr, L"BlendState ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
        return E_FAIL;
    }

    // ���÷� ����
    if (FAILED(CreateSampler()))
    {
        MessageBox(nullptr, L"���÷� ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
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
        MessageBox(NULL, L"Present() ȣ�� ����!", L"Error", MB_OK);
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
    DirectX���� **���� ü��(Swap Chain)**�� ������ ����� �����ϴ� �� ����(back buffer)�� �����ϴ� �����Դϴ�.
    ���� ü���� CPU�� �������� �������ϰ� GPU�� �̸� ȭ�鿡 ����ϴ� �������� **���� ���۸�(double buffering)
    */

    // ����ü�� ����
    DXGI_SWAP_CHAIN_DESC tDesc = {};

    tDesc.OutputWindow = m_hwnd;     // ��� ������
    tDesc.Windowed = true;           // â���, ��üȭ�� ���

    tDesc.BufferCount = 1; //���� ���۸�
    tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//�� ���۸� ������ Ÿ������ ���
    tDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;//�ػ� ����
    tDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
    tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//32��Ʈ(8��Ʈ�� 4ä��) ���� ���� (�Ϲ����� 8��Ʈ RGBA ����)
    tDesc.BufferDesc.RefreshRate.Denominator = 1;//�ֻ���(Hz)�� ���� (60Hz�� ����)
    tDesc.BufferDesc.RefreshRate.Numerator = 60;
    tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;//�Ϲ����� ���
    tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    tDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;//���� ������ ������ ����

    tDesc.SampleDesc.Count = 1;//(�⺻��)
    tDesc.SampleDesc.Quality = 0;
    tDesc.Flags = 0;

    // ����ü�� ����
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

//����Ÿ��
int CDevice::CreateView()
{
    //RenderTarget
    ComPtr<ID3D11Texture2D> pTex;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pTex.GetAddressOf());
   
    shared_ptr<CTexture> pRenderTex = CResMgr::GetInst()->CreateTexture(L"RenderTargetTex", pTex);
  
    //���̿� 24��Ʈ, ���ٽǿ� 8��Ʈ�� �����ϴ� 32��Ʈ z ���� �����Դϴ�.
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
 
    desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;//�޸�����
    desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;//���������� ������ �ﰢ���� ä��ϴ�. ������ �������� �׷����� �ʽ��ϴ�.
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
    //��� ����(Output-Merger)

    // No Blend
    m_BSState[(UINT)BS_TYPE::DEFAULT] = nullptr;

    //Src : �ȼ� ���̴����� ���� ��� ����
    //Dsc : ���� Ÿ�ٿ� �̹� �׷��� �ִ� ���� �ȼ��� ����

    // Mask
    D3D11_BLEND_DESC Desc = {};
    Desc.AlphaToCoverageEnable = true;                  //�ȼ� ���Ŀ� ���� ���� Ȱ�� 
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;            
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;  // �⺻ �� ���� (Src + Dest)
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;    // �ҽ� ���� 100%
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;  // ��� ���� 0%

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;  //���� ���� �ҽ��� ���.
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    m_device->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::MASK].GetAddressOf());


    // Alpha Blend
    //(SrcColor * SrcAlpha) + (DestColor * (1 - SrcAlpha))
    Desc.AlphaToCoverageEnable = false;                             // ���� ���� ��Ȱ��ȭ
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;              // �ҽ��� ��� ������ ��
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;          // �ҽ� ���İ� ���
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;     // (1 - �ҽ� ����)

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;         // ���İ� �ջ�
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;          // �ҽ� ���� 0%
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;          // ��� ���� 100%

    //���İ� ����� ���� ȿ���� ����
    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    m_device->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf());


    // One One
    //SrcColor + DestColor 1: 1 ���� ȥ�� : �� ���� ȿ��
    Desc.AlphaToCoverageEnable = false;
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;      // �ҽ��� ��� ������ ��
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;        // �ҽ� ���� 100%
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;       // ��� ���� 100%

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // ���İ� �ջ�
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;   // �ҽ� ���� 100%
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // ��� ���� 0%

    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    m_device->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());

    return S_OK;

}

int CDevice::CreateSampler()
{
    //�ؽ�ó�� ���ø�(�ȼ� �����͸� ��������)�� ���� ���� ���
    D3D11_SAMPLER_DESC tSamDesc = {};

    //�ؽ�ó ��ǥ�� 0~1 ������ ����� �ݺ�
    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.Filter = D3D11_FILTER_ANISOTROPIC;//�ؽ�ó�� �񽺵��� ���ų� Ȯ��/����� �� ǰ���� ����.
    tSamDesc.MaxLOD = D3D11_FLOAT32_MAX;
    m_device->CreateSamplerState(&tSamDesc, m_sampler[0].GetAddressOf());

    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;//�ؽ�ó�� ���ø��� �� ���� ����� �ؼ�(Texel) ���� �״�� ���.
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

