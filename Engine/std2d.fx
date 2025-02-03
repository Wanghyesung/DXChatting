#ifndef _STD2D
#define _STD2D

#include "value.fx"

struct VS_IN
{
    float3 vLocalPos : POSITIONT;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float3 vLocalPos : SV_Position;
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
    
    VS_OUT output = (VS_OUT)0;
    
    output.vLocalPos = mul(float4(_in.vLocalPos, 1.f), matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

// 레스터라이저 스테이트
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float output = (float4)0.f;
    
    if(btex_0)
    {
        if(int_0)
        {
           
           
            float2 vUV = vec2_0 + (vec2_3 * _in.vUV);
            vUV -= ((vec2_3 - vec2_1) / 2.f);
            vUV -= vec2_2;
           
            if (vec2_0.x < vUV.x && vec2_1.x < vec2_0.x + vec2_1.x
           && vec2_0.y < vUV.y && vec2_1.y < vec2_0.y + vec2_1.y)
           {
               output = tex_0.Sample(sam_0, vUV);
           }
           else
           {
                 //discard;
                discard;
                //output = float4(1.f, 1.f, 0.f, 1.f);
           }

        }
        else
        {
            output = tex_0.Sample(sam_0, _in.vUV);
        }
    }
    else
    {
        output = float4(1.f, 0.f, 1.f, 0.f);
    }
    
    return output;
}

#endif