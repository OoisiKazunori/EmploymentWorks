#include"../ShaderHeader/SpriteShaderHeader.hlsli"
#include"../ShaderHeader/MultiPassHeader.hlsli"

cbuffer lineData : register(b1)
{
    //xyz,color
    //w,distance
    float4 effectData;
}

TwoRender PSmain(VSOutput input) : SV_TARGET
{
    TwoRender output;
    float4 outputColor = float4(0.0f,0.0f,0.0f,1.0f);

    float upLimitRedLine = effectData.w + 0.01f;
    float downLimitRedLine = effectData.w - 0.01f;

    bool fillColorFlag = upLimitRedLine >= input.uv.y  && input.uv.y >= downLimitRedLine;
    if(fillColorFlag)
    {
        outputColor.xyz = effectData.xyz;
        output.target1 = float4(1.0f,0.0f,0.0f,1.0f);
    }
    else
    {
        outputColor.xyz =  float3(1.0f,1.0f,1.0f);
        output.target1 = float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    output.target0 = outputColor;
    return output;
}