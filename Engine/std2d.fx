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

#define bAnimUse        int_0
#define LeftTop         vec2_0
#define Slice           vec2_1
#define Offset          vec2_2
#define BackSize        vec2_3

VS_OUT VS_Std2D(VS_IN _in)
{
    
    VS_OUT output = (VS_OUT)0;
    
    output.vLocalPos = mul(float4(_in.vLocalPos, 1.f), matWVP);
    //output.vLocalPos = mul(output.vLocalPos, matView);
    //output.vLocalPos = mul(output.vLocalPos, matProj);
    output.vUV = _in.vUV;
   
    return output;
}

// �����Ͷ����� ������Ʈ
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float4 output = (float4)0.f;
   
    if(btex_0)
    {
        if (bAnimUse)
        {
          
           float2 vUV = LeftTop + (BackSize * _in.vUV);
           vUV -= ((BackSize - Slice) / 2.f);
           vUV -= Offset;
           
           if (LeftTop.x < vUV.x && vUV.x < LeftTop.x + Slice.x
               && LeftTop.y < vUV.y && vUV.y < LeftTop.y + Slice.y)
           {
               output = tex_0.Sample(sam_1, vUV);
           }
           else
           {
                //discard;
                output = float4(1.f, 1.f, 0.f, 1.f);
           }
        
        }
        else
        {
            output = tex_0.Sample(sam_1, _in.vUV);
        }
    }
    else
    {
        output = float4(1.f, 0.f, 1.f, 1.f);
    }
    
    return output;
}

#endif