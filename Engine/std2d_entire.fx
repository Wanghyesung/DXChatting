#ifndef _STD2D
#define _STD2D

#include "value.fx"

/*
1.���� �������� ����(Vertex Position)�� �־���
2.�� ���(View Matrix)�� ���Ͽ� ī�޶� �������� ��ȯ
3.���� ���(Projection Matrix)�� ���Ͽ� Ŭ���� ����(Clipping Space)���� ��ȯ
4.Ŭ���� ��, w ������ ������ NDC(-1 ~ 1) ��ȯ
5.����Ʈ ��ȯ(Viewport Transform) �� ���� ȭ�� ���
*/


struct VS_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vLocalPos : SV_Position; //���̴����� ��� ��ġ�� ����
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};


// ============================
// Std2DShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
// Parameter
// int_0              : AnimUse
// vec2_0             : AnimAtlas LeftTop
// vec2_1             : AnimAtlas Slice
//
// btex_0              : Output Texture
// ============================


VS_OUT VS_Std2D(VS_IN _in)
{
    
    VS_OUT output = (VS_OUT) 0;
    
    output.vLocalPos = float4(_in.vLocalPos * 2.f, 1.f);
    //output.vLocalPos = mul(output.vLocalPos, matView);
    //output.vLocalPos = mul(output.vLocalPos, matProj);
    output.vUV = _in.vUV;
   
    return output;
}

// �����Ͷ����� ������Ʈ
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float4 output = (float4) 0.f;
   
    if (btex_0)
    {
        output = tex_0.Sample(sam_1, _in.vUV);
    }
    else
    {
        output = float4(1.f, 0.f, 1.f, 1.f);
    }
    
    return output;
}

#endif