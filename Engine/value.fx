//const buffer
cbuffer TRANSFORM : register(b0)
{
    row_major matrix matWorld;
    row_major matrix matView;
    row_major matrix matProj;
    
    row_major matrix matWV;
    row_major matrix matWVP;
}

cbuffer MATERIAL : register(b1)
{  
    int int_0;
    int int_1;
    int int_2;
    int int_3;
    
    float float_0;
    float float_1;
    float float_2;
    float float_3;
    
    float2 vec2_0;
    float2 vec2_1;
    float2 vec2_2;
    float2 vec2_3;

    float4 vec4_0;
    float4 vec4_1;
    float4 vec4_2;
    float4 vec4_3;
    
    row_major matrix mat_0;
    row_major matrix mat_1;
    row_major matrix mat_2;
    row_major matrix mat_3;
    
    int btex_0;
    int btex_1;
    int btex_2;
    int btex_3;
    int btex_4;
    int btex_5;
    int btex_6;
    int btex_7;
    int btexcube_0;
    int btexcube_1;
    int btexarr_0;
    int btexarr_1;
    
    //Animation 정보
    //int g_iAnim; 
    //int3 padding;
};

cbuffer GLOBAL : register(b2)
{
    float2 g_Resolution;
    float g_DT;
    float g_AccTime;
    uint g_Light2DCount;
    int3 g_globalpadding;
}

Texture2D tex_0 : register(t0);
Texture2D tex_1 : register(t1);
Texture2D tex_2 : register(t2);
Texture2D tex_3 : register(t3);
Texture2D tex_4 : register(t4);
Texture2D tex_5 : register(t5);
Texture2D tex_6 : register(t6);
Texture2D tex_7 : register(t7);


SamplerState sam_0 : register(s0); //포인트
SamplerState sam_1 : register(s1);

